#pragma once

#include "io/BitReader.h"
#include "rc/RateController.h"
#include "model/ProbabilityModel.h"
#include "base/ArithmeticDecoder.h"


template <typename Symbol>
class RateDropArithmeticDecoder {
private:
	/*
	** Composition of these three objects.
	*/
	ProbabilityModel<Symbol>  &model;
	ArithmeticDecoder<Symbol> decoder;
	RateController            &rateController;


public:
	/*
	** Constructor.
	*/
	RateDropArithmeticDecoder(
		BitReader& reader,
		ProbabilityModel<Symbol>& model,
		RateController& rateController
	) :
		model(model),
		decoder(reader, model),
		rateController(rateController) {
	}

	/*
	** Determines whether the rate controller has the budget to decode the next
	**   symbol.
	*/
	bool canDecodeNextSymbol() const {
		double max_cost = model.getSubrange(model.getCostliestSymbol()).bitcost();
		double budgeted_cost = rateController.symbolBudget();
		return (max_cost <= budgeted_cost);
	}

	/*
	** Signals to the encoder that a symbol was skipped, allowing the rate
	**   contoller to adjust its budget accordingly.
	*/
	void skipSymbol() {
		rateController.spendBits();
	}

	/*
	** Decodes the next symbol.
	*/
	Symbol decode() {
		Symbol symbol(decoder.decode());
		rateController.spendBits(model.getSubrange(symbol).bitcost());
		model.update(symbol);
		return symbol;
	}
};
