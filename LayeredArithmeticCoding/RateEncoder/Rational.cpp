#include "Rational.h"


Rational::Rational(Number num, Number den) :
	num(num),
	den(den) {
	simplify();
}

Rational::Number Rational::gcd(Number a, Number b) {
	return b == 0 ? a : gcd(b, a % b);
}

Rational Rational::operator+(const Rational& rhs) const {
	return Rational(num*rhs.den + rhs.num*den, den*rhs.den);
}

Rational Rational::operator-(const Rational& rhs) const {
	return Rational(num*rhs.den - rhs.num*den, den*rhs.den);
}

Rational Rational::operator*(const Rational& rhs) const {
	return Rational(num*rhs.num, den*rhs.den);
}

Rational Rational::operator/(const Rational& rhs) const {
	return Rational(num*rhs.den, den*rhs.num);
}

bool Rational::operator<(const Rational& rhs) const {
	if (den*rhs.den > 0) {
		return (num*rhs.den < rhs.num*den);
	} else {
		return (num*rhs.den > rhs.num*den);
	}
}

bool Rational::operator>(const Rational& rhs) const {
	return !(*this < rhs) && !(*this == rhs);
}

bool Rational::operator<=(const Rational& rhs) const {
	return (*this < rhs) || (*this == rhs);
}

bool Rational::operator>=(const Rational& rhs) const {
	return !(*this < rhs);
}

bool Rational::operator==(const Rational& rhs) const {
	return num*rhs.den == rhs.num*den;
}

void Rational::simplify() {
	Number factor = gcd(num < 0 ? -num : num, den < 0 ? -den : den);
	num /= factor;
	den /= factor;
}
double Rational::evaluate() const {
	return static_cast<double>(num) / den;
}

ostream& operator<<(ostream &stream, const Rational &obj) {
	return stream << obj.num << '/' << obj.den;
}
