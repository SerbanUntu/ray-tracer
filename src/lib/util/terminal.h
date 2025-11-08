#pragma once
#include <cmath>
#include <iostream>

void clear_current_stdout_row();
void display_percentage(int current, int total, std::string qty, int width = 20);
