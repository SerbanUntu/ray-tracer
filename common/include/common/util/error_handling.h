#pragma once
#include <exception>

namespace raytracer::common
{
/**
 * Logs that an exception occurred.
 *
 * @param err The exception to log.
 */
void reportException(const std::exception &err) noexcept;

/**
 * Logs that an unknown exception occurred.
 */
void reportUnknownException() noexcept;
} // namespace raytracer::common
