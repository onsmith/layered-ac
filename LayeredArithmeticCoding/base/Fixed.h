#pragma once

#include <ostream>
using std::ostream;

#include <cstdint>


template <bool SIGNED, size_t PRECISION>
class Fixed {
public:
	/*
	** Underlying data type.
	*/
	typedef int64_t fixed_t;

	/*
	** Fractional mask.
	*/
	const static fixed_t PRECISION_MASK = (0x1 << PRECISION) - 1;

	/*
	** Define unity.
	*/
	const static fixed_t ONE = (0x1 << PRECISION);


private:
	/*
	** Underlying value.
	*/
	fixed_t value;


	/*
	** Internal factory method.
	*/
	static inline Fixed create(fixed_t value) {
		Fixed result(0);
		result.value = value;
		return result;
	}


public:
	/*
	** Constructors.
	*/
	Fixed(int64_t  integer)  : value(fixed_t(integer) << PRECISION    ) {}
	Fixed(uint64_t integer)  : value(fixed_t(integer) << PRECISION    ) {}
	Fixed(int      integer)  : value(fixed_t(integer) << PRECISION    ) {}
	Fixed(float    value  )  : value(static_cast<fixed_t>(value * ONE)) {}
	Fixed(double   value  )  : value(static_cast<fixed_t>(value * ONE)) {}


	/*
	** Access integer or fractional portion of value.
	*/
	fixed_t integer()  const { return (value >> PRECISION     ); }
	fixed_t fraction() const { return (value  & PRECISION_MASK); }


	/*
	** Comparison operators.
	*/
	inline bool operator==(const Fixed &rhs) const { return value == rhs.value; }
	inline bool operator!=(const Fixed &rhs) const { return value != rhs.value; }
	inline bool operator< (const Fixed &rhs) const { return value <  rhs.value; }
	inline bool operator> (const Fixed &rhs) const { return value >  rhs.value; }
	inline bool operator<=(const Fixed &rhs) const { return value <= rhs.value; }
	inline bool operator>=(const Fixed &rhs) const { return value >= rhs.value; }


	/*
	** Unary operators.
	*/
	inline bool   operator! () const { return !value;              }
	inline Fixed  operator~ () const { return Fixed(~value);       }
	inline Fixed& operator++()       { value += ONE; return *this; }
	inline Fixed& operator--()       { value += ONE; return *this; }


	/*
	** Multiplication.
	*/
	inline Fixed& operator*=(const Fixed &rhs) {
		value *= rhs.value;
		value >>= PRECISION;
		return *this;
	}


	/*
	** Division.
	*/
	inline Fixed &operator/=(const Fixed &rhs) {
		value <<= PRECISION;
		value /= rhs.value;
		return *this;
	}


	/*
	** Binary modifier operators.
	*/
	inline Fixed& operator+=(const Fixed &rhs) { value += rhs.value; return *this; }
	inline Fixed& operator-=(const Fixed &rhs) { value -= rhs.value; return *this; }
	inline Fixed& operator&=(const Fixed &rhs) { value &= rhs.value; return *this; }
	inline Fixed& operator|=(const Fixed &rhs) { value |= rhs.value; return *this; }
	inline Fixed& operator^=(const Fixed &rhs) { value ^= rhs.value; return *this; }


	/*
	** Binary arithmetic operators.
	*/
	inline Fixed operator+(const Fixed &rhs) const { Fixed x(*this); x *= rhs; return x; }
	inline Fixed operator-(const Fixed &rhs) const { Fixed x(*this); x -= rhs; return x; }
	inline Fixed operator&(const Fixed &rhs) const { Fixed x(*this); x &= rhs; return x; }
	inline Fixed operator|(const Fixed &rhs) const { Fixed x(*this); x |= rhs; return x; }
	inline Fixed operator^(const Fixed &rhs) const { Fixed x(*this); x ^= rhs; return x; }
	inline Fixed operator*(const Fixed &rhs) const { Fixed x(*this); x *= rhs; return x; }
	inline Fixed operator/(const Fixed &rhs) const { Fixed x(*this); x /= rhs; return x; }


	/*
	** Conversion methods.
	*/
	int    toInt()    const { return integer();                        }
	float  toFloat()  const { return static_cast<float >(value) / ONE; }
	double toDouble() const { return static_cast<double>(value) / ONE; }


	/*
	** Output stream operator.
	*/
	friend ostream& operator<<(ostream& stream, Fixed &rhs) {
		return stream << rhs.toDouble();
	}


	/*
	** Base 2 logarithm.
	*/
	static Fixed log2(const Fixed fp) {
		int64_t b  = 0x1 << (PRECISION - 1);
		int64_t y  = 0;
		int64_t x = fp.value;

		//if (precision < 1 || precision > 31) {
		//	errno = EINVAL;
		//	return INT32_MAX; // indicates an error
		//}

		//if (x == 0) {
		//		return INT32_MIN; // represents negative infinity
		//}

		while (x < 0x1 << PRECISION) {
			x <<= 1;
			y -= 0x1 << PRECISION;
		}

		while (x >= 0x2 << PRECISION) {
			x >>= 1;
			y += 0x1 << PRECISION;
		}

		int64_t z = x;

		for (size_t i = 0; i < PRECISION; i++) {
			z = z * z >> PRECISION;
			if (z >= 0x2 << PRECISION) {
				z >>= 1;
				y += b;
			}
			b >>= 1;
		}

		return create(y);
	}
};
