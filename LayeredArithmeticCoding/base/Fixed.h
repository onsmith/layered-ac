#pragma once

#include <ostream>
using std::ostream;

#include <cstdint>
#include <math.h>


template <bool SIGNED, size_t PRECISION>
class Fixed {
public:
	/*
	** Underlying data type.
	*/
	typedef int64_t value_t;


	/*
	** Define unity.
	*/
	const static value_t ONE      = (0x1 << PRECISION);
	const static int64_t TWO      = ONE * 2;
	const static int64_t ONE_HALF = ONE / 2;


private:
	/*
	** Underlying value.
	*/
	value_t value;


	/*
	** Internal factory method.
	*/
	static inline Fixed create(value_t value) {
		Fixed result(0);
		result.value = value;
		return result;
	}


public:
	/*
	** Constructors.
	*/
	Fixed(signed char        integer) : value(static_cast<value_t>(integer) << PRECISION) {}
	Fixed(signed short       integer) : value(static_cast<value_t>(integer) << PRECISION) {}
	Fixed(signed int         integer) : value(static_cast<value_t>(integer) << PRECISION) {}
	Fixed(signed long        integer) : value(static_cast<value_t>(integer) << PRECISION) {}
	Fixed(signed long long   integer) : value(static_cast<value_t>(integer) << PRECISION) {}

	Fixed(unsigned char      integer) : value(static_cast<value_t>(integer) << PRECISION) {}
	Fixed(unsigned short     integer) : value(static_cast<value_t>(integer) << PRECISION) {}
	Fixed(unsigned int       integer) : value(static_cast<value_t>(integer) << PRECISION) {}
	Fixed(unsigned long      integer) : value(static_cast<value_t>(integer) << PRECISION) {}
	Fixed(unsigned long long integer) : value(static_cast<value_t>(integer) << PRECISION) {}

	Fixed(float              value  ) : value(static_cast<value_t>(value * ONE)         ) {}
	Fixed(double             value  ) : value(static_cast<value_t>(value * ONE)         ) {}
	Fixed(long double        value  ) : value(static_cast<value_t>(value * ONE)         ) {}


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
	inline bool  operator!() const { return !value;         }
	inline Fixed operator~() const { return create(~value); }


	/*
	** Unary modifier operators.
	*/
	inline Fixed& operator++() { value += ONE; return *this; }
	inline Fixed& operator--() { value -= ONE; return *this; }


	/*
	** Binary operators.
	*/
	inline Fixed& operator+=(const Fixed &rhs) { value += rhs.value;                       return *this; }
	inline Fixed& operator-=(const Fixed &rhs) { value -= rhs.value;                       return *this; }
	inline Fixed& operator*=(const Fixed &rhs) { value *= rhs.value;  value >>= PRECISION; return *this; }
	inline Fixed& operator/=(const Fixed &rhs) { value <<= PRECISION; value /= rhs.value;  return *this; }

	inline Fixed& operator&=(const Fixed &rhs) { value &= rhs.value;                       return *this; }
	inline Fixed& operator|=(const Fixed &rhs) { value |= rhs.value;                       return *this; }
	inline Fixed& operator^=(const Fixed &rhs) { value ^= rhs.value;                       return *this; }


	/*
	** Binary modifier operators.
	*/
	inline Fixed operator+(const Fixed &rhs) const { Fixed x(*this); x *= rhs; return x; }
	inline Fixed operator-(const Fixed &rhs) const { Fixed x(*this); x -= rhs; return x; }
	inline Fixed operator*(const Fixed &rhs) const { Fixed x(*this); x *= rhs; return x; }
	inline Fixed operator/(const Fixed &rhs) const { Fixed x(*this); x /= rhs; return x; }

	inline Fixed operator&(const Fixed &rhs) const { Fixed x(*this); x &= rhs; return x; }
	inline Fixed operator|(const Fixed &rhs) const { Fixed x(*this); x |= rhs; return x; }
	inline Fixed operator^(const Fixed &rhs) const { Fixed x(*this); x ^= rhs; return x; }


	/*
	** Conversion methods.
	*/
	inline int    toInt()    const { return static_cast<int   >(value >> PRECISION); }
	inline float  toFloat()  const { return static_cast<float >(value) / ONE;        }
	inline double toDouble() const { return static_cast<double>(value) / ONE;        }


	/*
	** Output stream operator.
	*/
	friend ostream& operator<<(ostream& stream, const Fixed &rhs) {
		return stream << rhs.toDouble();
	}


	/*
	** Base 2 logarithm.
	*/
	static Fixed log2(Fixed x) {
		Fixed result(0);

		// Calculate integer portion of logarithm
		while (x.value <  ONE) { x.value <<= 1; --result; }
		while (x.value >= TWO) { x.value >>= 1; ++result; }

		// Calculate fractional portion of logarithm
		for (size_t i = 0; i < PRECISION; i++) {
			x *= x;
			if (x.value >= TWO) {
				x.value >>= 1;
				result.value += (ONE_HALF >> i);
			}
		}

		return result;
	}


	/*
	** Generic logarithm.
	*/
	static Fixed log(const Fixed& x, const Fixed& b) {
		return log2(x) / log2(b);
	}


	/*
	** Base 10 logarithm.
	*/
	static Fixed log10(const Fixed& x) {
		return log(x, 10);
	}


	/*
	** Natural logarithm.
	*/
	static Fixed ln(const Fixed& x) {
		return log(x, exp(1));
	}
};
