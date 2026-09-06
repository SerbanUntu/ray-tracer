#include "common/util/terminal.h"

constexpr auto ANSI_ESCAPE = '\33';
constexpr auto ANSI_CLEAR_ROW = "[2K";

void clear_current_stdout_row()
{
    std::cout << ANSI_ESCAPE << ANSI_CLEAR_ROW << '\r';
}

void display_percentage(const int current, const int total, const std::string& qty, const int width)
{
    const double fraction = static_cast<double>(current) / static_cast<double>(total);
    const int shown = static_cast<int>(std::round(fraction * static_cast<double>(width)));
    std::cout << '[';
    for (int i = 0; i < shown; i++)
    {
        std::cout << '=';
    }
    for (int i = shown; i < width; i++)
    {
        std::cout << ' ';
    }
    std::cout << "] "
        << static_cast<int>(std::round(fraction * 100))
        << "% ("
        << qty
        << ' '
        << current
        << " / "
        << total
        << ')';
}
