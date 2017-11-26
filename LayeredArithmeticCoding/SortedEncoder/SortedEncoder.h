#pragma once

#include "base/RateDropArithmeticEncoder.h"

#include <vector>
using std::vector;


template <typename Symbol>
class SortedEncoder {
private:
	/*
	** Composition of multiple arithmetic encoders.
	*/
	vector<RateDropArithmeticEncoder<Symbol>> &encoders;


public:
	/*
	** Constructor.
	*/
	SortedEncoder(
		vector<RateDropArithmeticEncoder<Symbol>> &encoders
	) :
		encoders(encoders) {
	}

	/*
	** Encodes the specified symbol.
	*/
	void encode(Symbol symbol) {
		bool hasBeenEncoded = false;
		for (int i = 0; i < encoders.size(); i++) {
			if (!hasBeenEncoded && encoders[i].canEncodeNextSymbol()) {
				encoders[i].encode(symbol);
				hasBeenEncoded = true;
				if (i == 0) {
					encoders[i].model().update(symbol);
				}
			} else {
				encoders[i].skipSymbol();
			}
		}
	}

	/*
	** Finishes encoding by flushing any pending bits.
	*/
	void finish() {
		for (int i = 0; i < encoders.size(); i++) {
			encoders[i].finish();
		}
	}
};
