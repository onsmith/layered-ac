#include "BinaryEncoder.h"

#include "ProbabilityRange.h"

#include <iostream>
using std::cout;
using std::endl;


BinaryEncoder::BinaryEncoder(BitReader &reader, BitWriter &writer) :
	reader(reader),
	writer(writer),
	pendingBitCounter(0) {
}

inline void BinaryEncoder::writeBit(bit_t bit) {
	writer.writeBit(bit);
}

inline void BinaryEncoder::addPendingBit() {
	pendingBitCounter++;
}

inline void BinaryEncoder::writePendingBits(bit_t bit) {
	while (pendingBitCounter > 0) {
		writeBit(bit);
		pendingBitCounter--;
	}
}

void BinaryEncoder::run() {
	code_value_t low  = 0;
	code_value_t high = WHOLE_RANGE;

	// Loop through input characters
	while (true) {
		// Read the next input symbol
		symbol_t c = reader.readBit();
		if (reader.eof()) {
			break;
		}

		// Shrink the encoding range according to the symbol's subinterval within
		//   the probability model
		ProbabilityRange p = model.getProbability(c);
		model.observe(c);
		code_value_t range = high - low + 1;
		high = low + (range * p.high / p.range) - 1;
		low  = low + (range * p.low  / p.range);

		// Output bits and bit-shift the encoding range according to how much it
		//   collapsed as a result of the input character.
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

	// Finish encoding
	addPendingBit();
	if (low < ONE_FOURTH_RANGE) {
		writeBit(0);
		writePendingBits(1);
	} else {
		writeBit(1);
		writePendingBits(0);
	}
}
