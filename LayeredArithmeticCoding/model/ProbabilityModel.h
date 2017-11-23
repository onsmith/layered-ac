#pragma once

#include "ProbabilityRange.h"

#include <cstdint>


template <typename Symbol>
class ProbabilityModel {
public:
	/*
	** Data types.
	*/
	typedef uint64_t Probability;


public:
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

	/*
	** Updates the model given the presence of a symbol. Allows ProbabilityModel
	**   objects to be adaptive.
	*/
	virtual void update(Symbol) = 0;

	/*
	** Looks up the cheapest symbol to encode.
	*/
	virtual Symbol getCheapestSymbol() const = 0;

	/*
	** Looks up the costliest symbol to encode.
	*/
	virtual Symbol getCostliestSymbol() const = 0;
};
