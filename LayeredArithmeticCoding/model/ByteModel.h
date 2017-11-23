#pragma once

#include "ProbabilityModel.h"

#include <vector>
using std::vector;


class ByteModel : public ProbabilityModel<unsigned char> {
public:
	/*
	** Data types.
	*/
	typedef unsigned char Symbol;


private:
	/*
	** Counts the number of observed symbols.
	*/
	vector<Probability> counts;

	/*
	** Stores the cheapest and costliest symbols.
	*/
	Symbol cheapest, costliest;


private:
	/*
	** Gets the number of occurences of a given symbol.
	*/
	Probability getCount(Symbol) const;


public:
	/*
	** Constructor.
	*/
	ByteModel();

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
