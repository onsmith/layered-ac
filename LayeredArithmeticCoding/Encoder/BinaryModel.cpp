#include "BinaryModel.h"


void BinaryModel::observe(symbol_t bit) {
	if (bit) {
		numOnes++;
	} else {
		numZeros++;
	}
}

ProbabilityRange BinaryModel::getProbability(symbol_t bit) const {
	long unsigned int numChars = getRange();
	if (bit) {
		return ProbabilityRange(numZeros, numChars, numChars);
	} else {
		return ProbabilityRange(0, numZeros, numChars);
	}
}

BinaryModel::probability_t BinaryModel::getRange() const {
	return numZeros + numOnes;
}

BinaryModel::symbol_t BinaryModel::getSymbol(probability_t value) const {
	return (value >= numZeros);
}
