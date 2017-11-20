#pragma once

#include "ProbabilityRange.h"

#include <cstdint>


template <typename Symbol>
class ProbabilityModel {
public:
	/*
	** Data types.
	*/
	typedef uint32_t Probability;

	/*
	** Gets the largest permitted Probability value.
	*/
	virtual Probability getRange() const = 0;

	/*
	** Looks up the ProbabilityRange for a given Symbol.
	*/
	virtual ProbabilityRange getSubrange(Symbol) const = 0;

	/*
	** Looks up the Symbol for a given Probability value.
	*/
	virtual Symbol getSymbol(Probability) const = 0;
};
