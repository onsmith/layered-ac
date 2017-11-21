#pragma once

#include "TargetRateController.h"

#include "io/BitWriter.h"
#include "model/ProbabilityModel.h"
#include "base/ArithmeticEncoder.h"


class RateBinaryArithmeticEncoder:
	public ArithmeticEncoder<bool>,
	public TargetRateController {
public:
	/*
	** Data types.
	*/
	typedef bool Symbol;


private:


public:
	/*
	** Constructor.
	*/
	RateBinaryArithmeticEncoder(BitWriter&, ProbabilityModel<Symbol>&, double target, double budget = 0.0);

	/*
	** Modify the encoder to keep track of the bit budget.
	*/
	void encode(Symbol);
};
