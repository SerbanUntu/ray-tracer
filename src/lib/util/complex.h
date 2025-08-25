#pragma once
#include <math.h>

class Complex {
private:
	double re;
	double im;
public:
	Complex(double _re, double _im) : re(_re), im(_im) {};
	static const Complex ZERO;

	double real_part() const { return re; }
	double imaginary_part() const { return im; }
	Complex squared() const { return Complex(re * re - im * im, 2 * re * im); }
	Complex conjugate() const { return Complex(re, -im); }
	double magnitude() const { return std::sqrt(re * re + im * im); }
	double magnitude_squared() const { return re * re + im * im; }

	Complex operator-() const { return Complex(-re, -im); }
	Complex operator+(Complex const& other) const {
		return Complex(re + other.re, im + other.im);
	}
	Complex operator-(Complex const& other) const {
		return Complex(re - other.re, im - other.im);
	}

};

inline const Complex Complex::ZERO = Complex(0, 0);
