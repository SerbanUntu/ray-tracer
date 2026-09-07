#pragma once
#include <iostream>

namespace raytracer::common
{
void clear_current_stdout_row();
void display_percentage(int current, int total, const std::string& qty, int width = 20);
} // namespace raytracer::common
