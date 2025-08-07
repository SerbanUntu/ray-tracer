#pragma once
#include<math.h>

class Vec3 {
private:
	double x;
	double y;
	double z;

public:
	Vec3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {
	}
	inline void normalize() {
		const int denominator = this->length();
		x /= denominator;
		y /= denominator;
		z /= denominator;
	}
	inline double length() const {
		return sqrt(x * x + y * y + z * z);
	}
};