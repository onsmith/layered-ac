#pragma once

#include <ostream>
using std::ostream;

struct Rational {
public:
	/*
	** Data type.
	*/
	typedef long long Number;


private:
	/*
	** Stores the numerator and denominator.
	*/
	Number num, den;


private:
	/*
	** Euclid's greatest common devisor algorithm.
	*/
	static Number gcd(Number, Number);


public:
	/*
	** Constructors.
	*/
	Rational(Number = 0, Number = 1);

	/*
	** Simplification methods.
	*/
	void simplify();

	/*
	** Evaluates the fraction, returning a floating-point number.
	*/
	double evaluate() const;

	/*
	** Operator overloads.
	*/
	Rational operator+(const Rational&) const;
	Rational operator-(const Rational&) const;
	Rational operator*(const Rational&) const;
	Rational operator/(const Rational&) const;

	bool operator<(const Rational&) const;
	bool operator>(const Rational&) const;
	bool operator<=(const Rational&) const;
	bool operator>=(const Rational&) const;
	bool operator==(const Rational&) const;

	friend ostream& operator<<(ostream&, const Rational&);
};
