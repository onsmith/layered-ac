#pragma once

#include <cstdint>


class ProbabilityRange {
public:
	/*
	** Represents the low and high bounds of the probability range.
	*/
	uint32_t low, high;

	/*
	** Represents the total number of units (in integer math) used to describe
	**   probabilities. Becomes the denominator used to divide low and high so
	**   that they lie in the range [0, 1].
	*/
	uint32_t count;
};
