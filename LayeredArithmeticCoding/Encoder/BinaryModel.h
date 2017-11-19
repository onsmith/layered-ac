#pragma once

#include "ProbabilityRange.h"


class BinaryModel {
public:
	/*
	** Data types.
	*/
	typedef bool              symbol_t;
	typedef long unsigned int probability_t;


private:
	/*
	** Counts the number of observed bits.
	*/
	probability_t numZeros = 1;
	probability_t numOnes  = 1;


public:
	/*
	** Observes a given symbol.
	*/
	void observe(symbol_t);

	/*
	** Gets the range of probabilities.
	*/
	probability_t getRange() const;

	/*
	** Looks up the current probability of a given symbol.
	*/
	ProbabilityRange getProbability(symbol_t) const;

	/*
	** Looks up the symbol for a given probability.
	*/
	symbol_t getSymbol(probability_t) const;
};
