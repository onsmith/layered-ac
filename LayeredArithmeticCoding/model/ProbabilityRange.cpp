#include "ProbabilityRange.h"


ProbabilityRange::ProbabilityRange(Probability low, Probability high, Probability range) :
	low(low),
	high(high),
	range(range) {
}

ProbabilityRange::fixed ProbabilityRange::bitcost() const {
	fixed tmp(range);
	return fixed::log2(tmp / (high - low));
}
