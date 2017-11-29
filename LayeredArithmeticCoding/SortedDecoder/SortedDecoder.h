#pragma once

#include "base/RateDropArithmeticDecoder.h"

#include <vector>
using std::vector;


template <typename Symbol>
class SortedDecoder : public Decoder<Symbol> {
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
	** Returns true iff the decoder has attempted to read past the end of the
	**   source data stream.
	*/
	bool eof() const final {
		bool eof = true;
		for (int i = 0; i < decoders.size(); i++) {
			eof = eof && decoders[i].eof();
		}
		return eof;
	}

	/*
	** Decodes the next symbol.
	*/
	Symbol decode() final {
		Symbol symbol = 'X';
		bool hasBeenDecoded = false;
		for (int i = 0; i < decoders.size(); i++) {
			if (!hasBeenDecoded && !decoders[i].eof() && decoders[i].canDecodeNextSymbol()) {
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
