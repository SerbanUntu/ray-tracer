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
	inline Vec2 to_normalized() const {
		const int denominator = this->length();
		return Vec2(x / denominator, y / denominator);
	}
	inline double length() const {
		return sqrt(x * x + y * y);
	}

	inline Vec2 operator+(Vec2 const& other) const {
		return Vec2(x + other.x, y + other.y);
	}
	inline Vec2 operator-(Vec2 const& other) const {
		return Vec2(x - other.x, y - other.y);
	}
	inline Vec2 operator-() const {
		return Vec2(-x, -y);
	}
	inline double operator*(Vec2 const& other) const {
		return x * other.x + y * other.y;
	}
	inline Vec2 operator*(double d) const {
		return Vec2(x * d, y * d);
	}
};