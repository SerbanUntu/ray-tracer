#pragma once
#include<math.h>

class Vec2 {
private:
	double x;
	double y;

public:
	Vec2(double _x, double _y) : x(_x), y(_y) {
	}
	inline void normalize() {
		const int denominator = this->length();
		x /= denominator;
		y /= denominator;
	}
	inline double length() const {
		return sqrt(x * x + y * y);
	}
};