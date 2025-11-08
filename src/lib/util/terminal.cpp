#include "terminal.h"

constexpr auto ANSI_ESCAPE = '\33';
constexpr auto ANSI_CLEAR_ROW = "[2K";

void clear_current_stdout_row() {
	std::cout << ANSI_ESCAPE << ANSI_CLEAR_ROW << '\r';
}

void display_percentage(int current, int total, std::string qty, int width) {
	double fraction = (double)current / (double)total;
	int shown = static_cast<int>(std::round(fraction * double(width)));
	std::cout << '[';
	for (int i = 0; i < shown; i++) {
		std::cout << '=';
	}
	for (int i = shown; i < width; i++) {
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
