#include "BinaryModel.h"


void BinaryModel::update(Symbol bit) {
	if (bit) {
		numOnes++;
	} else {
		numZeros++;
	}
}

ProbabilityRange BinaryModel::getSubrange(Symbol bit) const {
	long unsigned int numChars = getRange();
	if (bit) {
		return ProbabilityRange(numZeros, numChars, numChars);
	} else {
		return ProbabilityRange(0, numZeros, numChars);
	}
}

BinaryModel::Probability BinaryModel::getRange() const {
	return numZeros + numOnes;
}

BinaryModel::Symbol BinaryModel::getSymbol(Probability value) const {
	return (value >= numZeros);
}

BinaryModel::Symbol BinaryModel::getCheapestSymbol() const {
	return (numOnes > numZeros);
}

BinaryModel::Symbol BinaryModel::getCostliestSymbol() const {
	return (numOnes <= numZeros);
}
