#pragma once
#include <iostream>

namespace raytracer::common
{
inline std::ostream &get_nullout()
{
    static std::ostream stream(nullptr);
    return stream;
}

void clear_current_stdout_row(std::ostream &out);
void display_percentage(std::ostream &out, int current, int total, const std::string &qty, int width = 20);
} // namespace raytracer::common
