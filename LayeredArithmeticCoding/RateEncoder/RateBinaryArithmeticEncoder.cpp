#include "RateBinaryArithmeticEncoder.h"

#include <cmath>


RateBinaryArithmeticEncoder::RateBinaryArithmeticEncoder(BitWriter &writer, ProbabilityModel<Symbol> &model, double target, double budget) :
	ArithmeticEncoder<bool>(writer, model),
	TargetRateController(target, budget) {
}

void RateBinaryArithmeticEncoder::encode(Symbol symbol) {
	ArithmeticEncoder<bool>::encode(symbol);
	spendBits(model.getSubrange(symbol).bitcost());
}
