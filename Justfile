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

build:
    cmake --preset conan-release
    cmake --build --preset conan-release

test: build
    ctest --test-dir cmake-build-release/build/Release --output-on-failure

check:
    just check-format
    just install
    just build
    just lint
    just test

check-pipeline:
    act -j build -P ubuntu-latest=catthehacker/ubuntu:full-latest

fix:
    just format
    just install
    just build
    just lint-fix

docs:
    doxygen

docs-pipeline:
    act -j docs -P ubuntu-latest=catthehacker/ubuntu:full-latest --secret GITHUB_TOKEN="$GITHUB_TOKEN" --env GITHUB_REPOSITORY=SerbanUntu/ray-tracer --env GITHUB_SERVER_URL=https://github.com
