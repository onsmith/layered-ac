#include "BinaryDecoder.h"

#include <iostream>
using std::cout;
using std::endl;

#define CODE_VALUE_BITS 8


BinaryDecoder::BinaryDecoder(BitReader &reader, BitWriter &writer) :
	reader(reader),
	writer(writer) {
}

void BinaryDecoder::run() {
	code_value_t high = WHOLE_RANGE;
	code_value_t low  = 0;

	// Fill code value from bits from input
	code_value_t value = 0;
	for (int i = 0; i < CODE_VALUE_BITS; i++) {
		value <<= 1;
		value += (reader.readBit() ? 1 : 0);
	}

	// Loop through input bits
	while (true) {
		// Query the probability model to get the next symbol
		code_value_t range = high - low + 1;
		code_value_t scaled_value = ((value - low + 1) * model.getRange() - 1) / range;
		symbol_t c = model.getSymbol(scaled_value);
		ProbabilityRange p = model.getProbability(c);
		model.observe(c);
		writer.writeBit(c);

		// Update low and high according to the symbol's subrange within the
		//   probability model
		high = low + (range * p.high / p.range) - 1;
		low  = low + (range * p.low  / p.range);

		// Shift out matching highest-order bits in value, low, and high
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
			if (reader.eof()) { return; }
		}
	}
}
