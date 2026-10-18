check-format:
    find apps common mandelbrot raytracer -type f \( -name '*.cpp' -o -name '*.h' -o -name '*.hpp' \) -print0 | xargs -0 --no-run-if-empty clang-format --dry-run --Werror

format:
    find apps common mandelbrot raytracer -type f \( -name '*.cpp' -o -name '*.h' -o -name '*.hpp' \) -print0 | xargs -0 --no-run-if-empty clang-format -i

install:
    conan install . --build=missing --output-folder=cmake-build-release -c tools.cmake.cmaketoolchain:generator=Ninja

lint:
    find apps common mandelbrot raytracer -type f -name '*.cpp' -not -path '*/test/*' -print0 | xargs -0 --no-run-if-empty clang-tidy -p cmake-build-release/build/Release

lint-fix:
    find apps common mandelbrot raytracer -type f -name '*.cpp' -print0 | xargs -0 --no-run-if-empty clang-tidy -fix -p cmake-build-release/build/Release

build opt="3" fast_math="OFF":
    cmake --preset conan-release -DRAYTRACER_OPT_LEVEL={{opt}} -DRAYTRACER_FAST_MATH={{fast_math}}
    cmake --build --preset conan-release

test opt="3" fast_math="OFF": (build opt fast_math)
    ctest --test-dir cmake-build-release/build/Release --output-on-failure

check opt="3" fast_math="OFF":
    just check-format
    just install
    just build {{opt}} {{fast_math}}
    just lint
    just test {{opt}} {{fast_math}}

check-pipeline:
    act -j test -P ubuntu-latest=catthehacker/ubuntu:full-latest

fix:
    just format
    just install
    just build
    just lint-fix

docs:
    doxygen

docs-pipeline:
    act -j docs -P ubuntu-latest=catthehacker/ubuntu:full-latest --secret GITHUB_TOKEN="$GITHUB_TOKEN" --env GITHUB_REPOSITORY=SerbanUntu/ray-tracer --env GITHUB_SERVER_URL=https://github.com
