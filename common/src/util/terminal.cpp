#include "common/util/terminal.h"
#include <cmath>

namespace raytracer::common
{
static constexpr auto ANSI_ESCAPE = '\33';
static constexpr auto ANSI_CLEAR_ROW = "[2K";

void clear_current_row(std::ostream &out)
{
    out << ANSI_ESCAPE << ANSI_CLEAR_ROW << '\r';
}

void display_percentage(std::ostream &out, const int current, const int total, const std::string &qty, const int width)
{
    const double fraction = static_cast<double>(current) / static_cast<double>(total);
    const int shown = static_cast<int>(std::round(fraction * static_cast<double>(width)));
    out << '[';
    for (int i = 0; i < shown; i++)
    {
        out << '=';
    }
    for (int i = shown; i < width; i++)
    {
        out << ' ';
    }
    out << "] " << static_cast<int>(std::round(fraction * 100)) << "% (" << qty << ' ' << current << " / " << total
        << ')';
}
} // namespace raytracer::common
