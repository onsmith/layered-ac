#pragma once

#include "io/BitWriter.h"
#include "rc/RateController.h"
#include "model/ProbabilityModel.h"
#include "base/ArithmeticEncoder.h"


template <typename Symbol>
class RateDropArithmeticEncoder {
private:
	/*
	** Composition of these three objects.
	*/
	ProbabilityModel<Symbol>  &model;
	ArithmeticEncoder<Symbol> encoder;
	RateController            &rateController;


public:
	/*
	** Constructor.
	*/
	RateDropArithmeticEncoder(
		BitWriter& writer,
		ProbabilityModel<Symbol>& model,
		RateController& rateController
	) :
		model(model),
		encoder(writer, model),
		rateController(rateController) {
	}

	/*
	** Determines whether the rate controller has the budget to encode the next
	**   symbol.
	*/
	bool canEncodeNextSymbol() const {
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
	** Encodes the specified symbol.
	*/
	void encode(Symbol symbol) {
		encoder.encode(symbol);
		rateController.spendBits(model.getSubrange(symbol).bitcost());
		model.update(symbol);
	}

	/*
	** Finishes encoding by flushing any pending bits.
	*/
	void finish() {
		encoder.finish();
	}
};
