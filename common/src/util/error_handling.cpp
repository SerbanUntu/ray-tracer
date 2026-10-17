#include "common/util/error_handling.h"

#include <string_view>
#include <cstdio>

namespace raytracer::common
{
void reportException(const std::exception &err) noexcept
{
    const std::string_view MSG_PREFIX = "The application failed: ";
    std::fprintf(stderr, "%.*s%s\n", static_cast<int>(MSG_PREFIX.size()), MSG_PREFIX.data(), err.what());
}

void reportUnknownException() noexcept
{
    const std::string_view MSG = "The application failed with an unknown exception.";
    std::fprintf(stderr, "%.*s\n", static_cast<int>(MSG.size()), MSG.data());
}
} // namespace raytracer::common