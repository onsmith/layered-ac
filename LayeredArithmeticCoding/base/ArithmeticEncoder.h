#pragma once

#include "model/ProbabilityModel.h"
#include "io/BitWriter.h"


template <typename Symbol>
class ArithmeticEncoder {
public:
	/*
	** Data types.
	*/
	typedef unsigned int Code;
	typedef bool         Bit;

	/*
	** Constants.
	*/
	const static int  CODE_BIT_LENGTH     = 8;
	const static Code WHOLE_RANGE         = (0x1 << CODE_BIT_LENGTH) - 1;
	const static Code ONE_HALF_RANGE      = 0x1 << (CODE_BIT_LENGTH - 1);
	const static Code ONE_FOURTH_RANGE    = ONE_HALF_RANGE >> 1;
	const static Code THREE_FOURTHS_RANGE = ONE_HALF_RANGE + ONE_FOURTH_RANGE;


protected:
	/*
	** Encapsulates a BitWriter for writing bits to an istream.
	*/
	BitWriter &writer;

	/*
	** Encapsulates a ProbabilityModel for computing symbol subranges.
	*/
	const ProbabilityModel<Symbol> &model;

	/*
	** Tracks the number of pending bits.
	*/
	int pendingBitCounter;

	/*
	** Tracks the arithmetic encoding range.
	*/
	Code low, high;


private:
	/*
	** Writes a single bit to the output stream.
	*/
	void writeBit(Bit bit) {
		writer.writeBit(bit);
	}

	/*
	** Adds a pending bit to the pending bit counter.
	*/
	void addPendingBit() {
		pendingBitCounter++;
	}

	/*
	** Writes the pending bits to the output stream and resets the pending bit
	**   counter.
	*/
	void writePendingBits(Bit bit) {
		while (pendingBitCounter > 0) {
			writeBit(bit);
			pendingBitCounter--;
		}
	}


public:
	/*
	** Constructor.
	*/
	ArithmeticEncoder(BitWriter &writer, ProbabilityModel<Symbol> &model) :
		writer(writer),
		model(model),
		low(0),
		high(WHOLE_RANGE),
		pendingBitCounter(0) {
	}

	/*
	** Finishes encoding by flushing any pending bits.
	*/
	void finish() {
		addPendingBit();
		if (low < ONE_FOURTH_RANGE) {
			writeBit(0);
			writePendingBits(1);
		} else {
			writeBit(1);
			writePendingBits(0);
		}
	}

	/*
	** Encodes a single symbol.
	*/
	void encode(Symbol symbol) {
		// Get the ProbabilityRange corresponding to the symbol
		ProbabilityRange p = model.getSubrange(symbol);

		// Shrink the encoding range according to the Symbol's ProbabilityRange
		Code const range = high - low + 1;
		high = low + (range * p.high / p.range) - 1;
		low  = low + (range * p.low  / p.range);

		// Output bits and bit-shift the encoding range according to how much it
		//   shrunk
		while (true) {
			if (high < ONE_HALF_RANGE) {
				writeBit(0);
				writePendingBits(1);
			} else if (low >= ONE_HALF_RANGE) {
				writeBit(1);
				writePendingBits(0);
			} else if (low >= ONE_FOURTH_RANGE && high < THREE_FOURTHS_RANGE) {
				addPendingBit();
				low  -= ONE_FOURTH_RANGE;
				high -= ONE_FOURTH_RANGE;
			} else {
				break;
			}
			high <<= 1;
			high++;
			low <<= 1;
			high &= WHOLE_RANGE;
			low  &= WHOLE_RANGE;
		}
	}

	/*
	** Virtual destructor.
	*/
	virtual ~ArithmeticEncoder() = default;
};
