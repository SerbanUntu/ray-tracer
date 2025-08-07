#pragma once
#include<math.h>

class Vec3 {
public:
	double x;
	double y;
	double z;

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

	Vec3 operator+(Vec3 const& other) {
		return Vec3(x + other.x, y + other.y, z + other.z);
	}
	Vec3 operator-(Vec3 const& other) {
		return Vec3(x - other.x, y - other.y, z - other.z);
	}
	double operator*(Vec3 const& other) {
		return x * other.x + y * other.y + z * other.z;
	}
};