#pragma once
#include <string>

class Color {
private:
	int r;
	int g;
	int b;

public:
	Color() : r(0), g(0), b(0) {
	}
	Color(int _r, int _g, int _b) : r(_r), g(_g), b(_b) {
	}
	int get_red() const { return r; }
	int get_green() const { return g; }
	int get_blue() const { return b; }
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

	static const Color RED;
	static const Color GREEN;
	static const Color BLUE;
};