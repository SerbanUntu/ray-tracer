# Metamorphic testing

A renderer has no reliable expected output: for any non-trivial scene, there is no independent way to know what the
correct image looks like, short of writing a second renderer. This is the *test oracle problem*. Metamorphic testing
sidesteps it by not checking a single output against a known answer, but instead checking how the output should change
(or stay the same) when the input is transformed in a known way. For example, moving the entire scene and the camera by
the same offset should not change the image, whatever that image is. The approach is well established for graphics
software. GraphicsFuzz, for instance, applies it to find bugs in shader compilers.

The ray tracer adds a second difficulty: it is a Monte Carlo renderer, so every image contains sampling noise. A useful
test therefore has to distinguish a real change in the picture from a different arrangement of noise.

## Approaches

### Comparing images

Images are compared in memory, on the floating-point colour values produced by the renderer, before they are quantized
to 8-bit BMP output. Quantization would both hide small real differences and exaggerate tiny ones that happen to cross a
colour bucket boundary.

The similarity metric (`common/include/common/image_similarity.h`) splits both images into square blocks (size 4x4 by default), averages the
colour of each block, and returns the mean Euclidean distance between corresponding block averages, weighted by block
area so that partial blocks at the image edges count proportionally. Averaging over blocks acts as a low-pass filter:
per-pixel sampling noise largely cancels out, while structural differences, such as a missing or displaced object,
remain. A downsampling factor of 1 means plain per-pixel mean distance.

### Two kinds of checks

Each relation is checked in one of two ways, depending on what it can guarantee:

- **Exact**: the transformed render must be bit-identical to the original. This applies
  when the transformation changes neither the geometry each ray sees nor the sequence of random numbers it consumes.
- **Tolerance-based**: the difference must stay below a threshold. This applies when the transformation
  changes the noise pattern, for example because random scattering directions are generated in world space.

The tolerance threshold was measured experimentally. Rendering the same scene with different random number streams gives
a number between 0.009-0.010 at 4 rays per pixel and a block size of 4. The tolerance was set to twice that (0.02).
To confirm that the threshold still has detection power, the rotation test was deliberately broken by rotating only the camera.
The difference rose to 0.089, about 4.5x the tolerance.

The random number generator is seeded with a fixed value in tests. This makes failures reproducible, and it is what
makes the exact check possible at all: with the same seed and thread count, rendering is fully deterministic.

### Relations

**Ray tracer** (`raytracer/test/test_renderer.cpp`):

| Relation                                                                          | Check            |
|-----------------------------------------------------------------------------------|------------------|
| Rotating the scene and the camera together by 90 degrees around the vertical axis | Within tolerance |
| Translating the scene and the camera together by a large offset (100000)          | Within tolerance |
| Replacing a diffuse material with a textured one of the same constant colour      | Exact            |
| Adding an object fully enclosed by another opaque object                          | Exact            |
| Reordering the scene's object list                                                | Exact            |
| Adding an object behind the camera                                                | Within tolerance |
| Rendering with 1, 2, 4 and 8 parallel threads                                     | Within tolerance |

**Mandelbrot set** (`mandelbrot/test/test_renderer.cpp`): rendering a region as two halves, or as four quadrants at
twice the zoom, and stitching the parts together must reproduce the full render exactly.

## Results

- **Order-dependent shading.** The object-reordering relation exposed a bug in the core tracing loop. Shading was
  computed for every intersection that was closer than the previous best, instead of only for the nearest one. Each
  discarded shading call recursively traced a full bounce path, wasting work, and consumed random numbers, so the noise
  in the image depended on the order in which objects were listed. The bug did not bias the image, so a
  tolerance-based check could not have caught it: the difference was 0.0104, inside the 0.02 tolerance. It was caught
  only when the relation was checked exactly, under a fixed seed. Shading the nearest hit once, after the
  intersection loop, made the reordered render bit-identical.
- **Floating-point exactness.** As described above, the Mandelbrot tiling relation is exact in real arithmetic but not
  in floating point unless the parameters are chosen carefully, which turned a naive tolerance-based test into a much
  stronger exact one.
- **Translation is exact in practice.** Translating the scene by 10⁵ produced a bit-identical image. In this renderer,
  colour depends only on which objects a light path hits, not on the exact hit positions, so rounding at large
  coordinates never changed the outcome for these scenes.
- **Stable across compilers and optimization levels.** All relations, including the exact ones, pass in every
  continuous integration configuration: GCC and Clang, at `-O0` and `-O3`, each with and without `-ffast-math`.

## Next steps

- **Differential testing across builds.** Render a fixed set of scenes with every compiler and optimization
  configuration, and compare the images between configurations, rather than only checking relations within each build.
  Without `-ffast-math`, renders from the same compiler are expected to be identical.
- **Randomly generated scenes.** Replace the handwritten scenes with randomly generated ones, using property-based
  testing to produce transformations whose effect is known by construction (for example, placing a new object fully
  inside an existing one). Shrinking would automatically minimize failing scenes to the smallest one that still fails.
- **Per-pixel seeding.** Seed the random number generator from each pixel's coordinates instead of once per thread.
  This would make the thread-count relation exact, and would stop a single extra random draw from shifting the noise
  of every later pixel, letting relations such as the object behind the camera be measured precisely.
