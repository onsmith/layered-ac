#include "ProbabilityRange.h"

#include <cmath>


ProbabilityRange::ProbabilityRange(uint32_t low, uint32_t high, uint32_t range) :
	low(low),
	high(high),
	range(range) {
}

double ProbabilityRange::bitcost() const {
	return log2(static_cast<double>(range) / (high - low));
}
