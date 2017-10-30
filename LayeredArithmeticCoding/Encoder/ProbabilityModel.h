#pragma once

#include <cstdint>

#include "ProbabilityRange.h"


class ProbabilityModel {
public:
	/*
	** Symbol data type.
	*/
	typedef uint8_t Symbol;

	/*
	** Computes the probability range of a given symbol (for encoding).
	*/
	virtual ProbabilityRange getProbabilityRange(Symbol) const = 0;

	/*
	** Computes the probability range of a given symbol (for decoding).
	*/
	virtual Symbol getSymbol(Symbol) const = 0;

	/*
	** Virtual destructor.
	*/
	virtual ~ProbabilityModel() = default;
};

