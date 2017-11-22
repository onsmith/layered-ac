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
	** Updates the model given the presence of a symbol.
	*/
	void update(Symbol) final;

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

	/*
	** Looks up the cheapest symbol to encode.
	*/
	Symbol getCheapestSymbol() const final;

	/*
	** Looks up the costliest symbol to encode.
	*/
	Symbol getCostliestSymbol() const final;
};
