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
	ProbabilityModel<Symbol>  &probabilityModel;
	ArithmeticDecoder<Symbol> decoder;
	RateController            &rateController;


public:
	/*
	** Constructor.
	*/
	RateDropArithmeticDecoder(
		BitReader& reader,
		ProbabilityModel<Symbol>& probabilityModel,
		RateController& rateController
	) :
		probabilityModel(probabilityModel),
		decoder(reader, probabilityModel),
		rateController(rateController) {
	}

	/*
	** Determines whether the rate controller has the budget to decode the next
	**   symbol.
	*/
	bool canDecodeNextSymbol() const {
		double max_cost = probabilityModel.getSubrange(probabilityModel.getCostliestSymbol()).bitcost();
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
		rateController.spendBits(probabilityModel.getSubrange(symbol).bitcost());
		//model.update(symbol);
		return symbol;
	}

	/*
	** Returns a reference to the model used by the decoder.
	*/
	ProbabilityModel<Symbol>& model() {
		return probabilityModel;
	}
};
