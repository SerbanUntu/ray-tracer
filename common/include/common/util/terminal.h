#pragma once
#include <iostream>

namespace raytracer::common
{
/**
 * Returns a reference to a std::ostream that does nothing. Used for tests.
 *
 * @return A reference to a std::ostream that does nothing.
 */
inline std::ostream &get_nullout()
{
    static std::ostream stream(nullptr);
    return stream;
}

/**
 * Writes an ANSI sequence to clear the current row from a terminal display.
 *
 * @param out The std::ostream to write to.
 */
void clear_current_row(std::ostream &out);
/**
 * Write a progress bar to the terminal.
 *
 * @param out The std::ostream to write to.
 * @param current The number of completed tasks.
 * @param total The total amount of tasks to complete.
 * @param qty What the percentage is measured for (used as a label).
 * @param width The width of the progress bar in characters.
 */
void display_percentage(std::ostream &out, int current, int total, const std::string &qty, int width = 20);
} // namespace raytracer::common
