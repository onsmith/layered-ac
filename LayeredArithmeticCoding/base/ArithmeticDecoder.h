#pragma once

#include "model/ProbabilityModel.h"
#include "io/BitReader.h"
#include "base/Decoder.h"


template <typename Symbol>
class ArithmeticDecoder : public Decoder<Symbol> {
public:
	/*
	** Data types.
	*/
	typedef unsigned int Code;
	typedef bool         Bit;

	/*
	** Constants.
	*/
	const static int  CODE_BIT_LENGTH     = 16;
	const static Code WHOLE_RANGE         = (0x1 << CODE_BIT_LENGTH) - 1;
	const static Code ONE_HALF_RANGE      = 0x1 << (CODE_BIT_LENGTH - 1);
	const static Code ONE_FOURTH_RANGE    = ONE_HALF_RANGE >> 1;
	const static Code THREE_FOURTHS_RANGE = ONE_HALF_RANGE + ONE_FOURTH_RANGE;


protected:
	/*
	** Encapsulates a BitReader for reading bits from an istream.
	*/
	BitReader &reader;

	/*
	** Encapsulates a ProbabilityModel for computing symbol subranges.
	*/
	const ProbabilityModel<Symbol> &probabilityModel;

	/*
	** Tracks the arithmetic encoding range.
	*/
	Code low, high;

	/*
	** Tracks the encoded value within the arithmetic encoding range.
	*/
	Code value;

	/*
	** Tracks remaining bits once the end of the input stream is reached.
	*/
	int bitsLeft = CODE_BIT_LENGTH;


private:
	/*
	** Initializes the encoded value by filling it with bits from the BitReader.
	*/
	void initializeValue() {
		value = 0;
		for (int i = 0; i < CODE_BIT_LENGTH; i++) {
			value <<= 1;
			value += (reader.readBit() ? 1 : 0);
		}
	}


public:
	/*
	** Constructor.
	*/
	ArithmeticDecoder(BitReader &reader, ProbabilityModel<Symbol> &probabilityModel) :
		reader(reader),
		probabilityModel(probabilityModel),
		low(0),
		high(WHOLE_RANGE) {
		initializeValue();
	}

	/*
	** Returns true iff the decoder has attempted to read past the end of the
	**   source data stream.
	*/
	bool eof() const final {
		return (bitsLeft <= 0);
		//return reader.eof();
	}

	/*
	** Decodes a single symbol, returning the probability range of the symbol
	**   observed.
	*/
	Symbol decode() final {
		// Get the Symbol and ProbabilityRange corresponding to the current Code
		Code const range = high - low + 1;
		Code const scaled_value = ((value - low + 1) * probabilityModel.getRange() - 1) / range;
		Symbol const symbol = probabilityModel.getSymbol(scaled_value);
		ProbabilityRange const p = probabilityModel.getSubrange(symbol);

		// Update low and high according to the symbol's subrange within the
		//   probability model
		high = low + (range * p.high / p.range) - 1;
		low  = low + (range * p.low  / p.range);

		// Bit-shift matching highest-order bits in value, low, and high
		while (true) {
			if (high < ONE_HALF_RANGE) {
			} else if (low >= ONE_HALF_RANGE) {
				value -= ONE_HALF_RANGE;
				low   -= ONE_HALF_RANGE;
				high  -= ONE_HALF_RANGE;
			} else if (low >= ONE_FOURTH_RANGE && high < THREE_FOURTHS_RANGE) {
				value -= ONE_FOURTH_RANGE;
				low   -= ONE_FOURTH_RANGE;
				high  -= ONE_FOURTH_RANGE;
			} else {
				break;
			}
			low   <<= 1;
			high  <<= 1;
			value <<= 1;
			high++;
			value += (reader.readBit() ? 1 : 0);
			if (reader.eof()) {
				bitsLeft--;
			}
		}

		return symbol;
	}
};
