#pragma once

#include "base/RateDropArithmeticDecoder.h"

#include <vector>
using std::vector;


template <typename Symbol>
class SortedDecoder {
private:
	/*
	** Composition of multiple arithmetic decoders.
	*/
	vector<RateDropArithmeticDecoder<Symbol>> &decoders;


public:
	/*
	** Constructor.
	*/
	SortedDecoder(
		vector<RateDropArithmeticDecoder<Symbol>> &decoders
	) :
		decoders(decoders) {
	}

	/*
	** Decodes the next symbol.
	*/
	Symbol decode() {
		Symbol symbol = 'X';
		bool hasBeenDecoded = false;
		for (int i = 0; i < decoders.size(); i++) {
			if (!hasBeenDecoded && decoders[i].canDecodeNextSymbol()) {
				symbol = decoders[i].decode();
				hasBeenDecoded = true;
				if (i == 0) {
					decoders[i].model().update(symbol);
				}
			} else {
				decoders[i].skipSymbol();
			}
		}
		return symbol;
	}
};
