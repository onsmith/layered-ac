#include "RateBinaryArithmeticDecoder.h"


RateBinaryArithmeticDecoder::RateBinaryArithmeticDecoder(BitReader &reader, ProbabilityModel<Symbol> &model, double target, double budget) :
	ArithmeticDecoder<bool>(reader, model),
	TargetRateController(target, budget) {
}

RateBinaryArithmeticDecoder::Symbol RateBinaryArithmeticDecoder::decode() {
	Symbol symbol(ArithmeticDecoder<bool>::decode());
	spendBits(model.getSubrange(symbol).bitcost());
	return symbol;
}
