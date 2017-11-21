#pragma once

#include "io/BitReader.h"
#include "model/ProbabilityModel.h"
#include "base/ArithmeticDecoder.h"
#include "RateEncoder/TargetRateController.h"


class RateBinaryArithmeticDecoder:
	public ArithmeticDecoder<bool>,
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
	RateBinaryArithmeticDecoder(BitReader&, ProbabilityModel<Symbol>&, double target, double budget = 0.0);

	/*
	** Modify the decoder to keep track of the bit budget.
	*/
	Symbol decode();
};
