#pragma once
#include<math.h>

class Vec3 {
public:
	double x;
	double y;
	double z;

	Vec3(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
	void normalize() {
		const double denominator = this->length();
		x /= denominator;
		y /= denominator;
		z /= denominator;
	}
	Vec3 to_normalized() const {
		const double denominator = this->length();
		return Vec3(x / denominator, y / denominator, z / denominator);
	}
	double length() const {
		return sqrt(x * x + y * y + z * z);
	}

	Vec3 operator+(Vec3 const& other) const {
		return Vec3(x + other.x, y + other.y, z + other.z);
	}
	void operator+=(Vec3 const& other) {
		x += other.x;
		y += other.y;
		z += other.z;
	}
	Vec3 operator-(Vec3 const& other) const {
		return Vec3(x - other.x, y - other.y, z - other.z);
	}
	Vec3 operator-() const {
		return Vec3(-x, -y, -z);
	}
	double operator*(Vec3 const& other) const {
		return x * other.x + y * other.y + z * other.z;
	}
	Vec3 operator*(double d) const {
		return Vec3(x * d, y * d, z * d);
	}
	Vec3 operator/(double d) const {
		return Vec3(x / d, y / d, z / d);
	}
};