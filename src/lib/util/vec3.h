#pragma once
#include<math.h>
#include<iostream>

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

	Vec3 cross(Vec3 const& other) const {
		return Vec3(
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x
		);
	}
	bool operator==(Vec3 const& other) const {
		return x == other.x && y == other.y && z == other.z;
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
	friend std::ostream& operator<<(std::ostream& os, Vec3 const& v) {
		return os << "Vec3(" << v.x << ", " << v.y << ", " << v.z << ")";
	}
	static const Vec3 ZERO;
};

const Vec3 Vec3::ZERO = Vec3(0, 0, 0);
