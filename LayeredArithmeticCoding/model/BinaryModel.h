#pragma once

#include "ProbabilityModel.h"


class BinaryModel : public ProbabilityModel<bool> {
public:
	/*
	** Data types.
	*/
	typedef bool Symbol;


private:
	/*
	** Counts the number of observed bits.
	*/
	Probability numZeros = 1;
	Probability numOnes  = 1;


public:
	/*
	** Observes a given symbol.
	*/
	void observe(Symbol);

	/*
	** Gets the range of probabilities.
	*/
	Probability getRange() const final;

	/*
	** Looks up the current probability of a given symbol.
	*/
	ProbabilityRange getSubrange(Symbol) const final;

	/*
	** Looks up the symbol for a given probability.
	*/
	Symbol getSymbol(Probability) const final;
};
