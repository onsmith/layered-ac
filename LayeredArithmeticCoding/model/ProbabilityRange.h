#pragma once

#include <cstdint>

#include "base/fixed.h"


struct ProbabilityRange {
public:
	/*
	** Data types.
	*/
	typedef Fixed<false, 20> fixed;
	typedef uint64_t         Probability;

	/*
	** Define the precision used for fixed point values.
	*/
	static const int FIXED_POINT_PRECISION = 20;


public:
	/*
	** Represents the low and high bounds of the probability range.
	*/
	Probability low, high;

	/*
	** Represents the total number of units (in integer math) used to describe
	**   probabilities. Becomes the denominator used to divide low and high so
	**   that they lie in the range [0, 1].
	*/
	Probability range;

	/*
	** Constructor.
	*/
	ProbabilityRange(Probability, Probability, Probability);

	/*
	** Calculates the bit cost of encoding the subrange.
	*/
	fixed bitcost() const;
};
