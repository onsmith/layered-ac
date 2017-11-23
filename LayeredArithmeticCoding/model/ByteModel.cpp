#include "ByteModel.h"


ByteModel::ByteModel() :
	costliest(0xFF),
	cheapest(0x0),
	counts(0x100) {
	counts[0] = 1;
	for (int i = 1; i < counts.size(); i++) {
		counts[i] = counts[i-1] + 1;
	}
}

void ByteModel::update(Symbol symbol) {
	if (counts.back() < 0xFFF) {
		for (int i = symbol; i < counts.size(); i++) {
			counts[i]++;
		}
		if (symbol == costliest) {
			for (int i = 0; i < counts.size(); i++) {
				if (getCount(i) < getCount(costliest)) {
					costliest = i;
				}
			}
		}
		if (getCount(symbol) > getCount(cheapest)) {
			cheapest = symbol;
		}
	}
}

ProbabilityRange ByteModel::getSubrange(Symbol symbol) const {
	if (symbol > 0) {
		return ProbabilityRange(counts[symbol-1], counts[symbol], counts.back());
	} else {
		return ProbabilityRange(0, counts[0], counts.back());
	}
}

ByteModel::Probability ByteModel::getRange() const {
	return counts.back();
}

ByteModel::Symbol ByteModel::getSymbol(Probability value) const {
	int low = -1, high = 255;
	while (high - low > 1) {
		int mid = (high + low) / 2;
		if (value < counts[mid]) {
			high = mid;
		} else {
			low = mid;
		}
	}
	return high;
}

ByteModel::Symbol ByteModel::getCheapestSymbol() const {
	return cheapest;
}

ByteModel::Symbol ByteModel::getCostliestSymbol() const {
	return costliest;
}

ByteModel::Probability ByteModel::getCount(Symbol symbol) const {
	if (symbol > 0) {
		return counts[symbol] - counts[symbol-1];
	} else {
		return counts[0];
	}
}
