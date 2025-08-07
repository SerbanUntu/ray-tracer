#pragma once
#include<math.h>

class Vec2 {
public:
	double x;
	double y;

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

	Vec2 operator+(Vec2 const& other) {
		return Vec2(x + other.x, y + other.y);
	}
	Vec2 operator-(Vec2 const& other) {
		return Vec2(x - other.x, y - other.y);
	}
	double operator*(Vec2 const& other) {
		return x * other.x + y * other.y;
	}
};