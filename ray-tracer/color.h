#pragma once
#include <string>

class Color {
private:
	short r;
	short g;
	short b;

public:
	Color() : r(0), g(0), b(0) {
	}
	Color(short _r, short _g, short _b) : r(_r), g(_g), b(_b) {
	}
	short get_red() const { return r; }
	short get_green() const { return g; }
	short get_blue() const { return b; }
	inline std::string to_string() const {
		std::string result = "RGB(";
		result += std::to_string(r);
		result += ", ";
		result += std::to_string(g);
		result += ", ";
		result += std::to_string(b);
		result += ")";
		return result;
	}

	inline Color operator*(double d) const {
		return Color((double) r * d, (double) g * d, (double) b * d);
	}

	static const Color RED;
	static const Color GREEN;
	static const Color BLUE;
};