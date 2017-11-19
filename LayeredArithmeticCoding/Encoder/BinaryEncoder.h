#pragma once

#include "BinaryModel.h"

#include "io/BitReader.h"
#include "io/BitWriter.h"

class BinaryEncoder {
public:
	/*
	** Data types.
	*/
	typedef unsigned int code_value_t;
	typedef bool         bit_t;
	typedef bool         symbol_t;

	/*
	** Constants.
	*/
	const static code_value_t WHOLE_RANGE         = 0xFF;
	const static code_value_t ONE_HALF_RANGE      = 0x80;
	const static code_value_t ONE_FOURTH_RANGE    = ONE_HALF_RANGE >> 1;
	const static code_value_t THREE_FOURTHS_RANGE = ONE_HALF_RANGE + ONE_FOURTH_RANGE;


private:
	/*
	** Encapsulates a BitReader for reading bits from an istream.
	*/
	BitReader &reader;
	
	/*
	** Encapsulates a BitWriter for writing bits to an istream.
	*/
	BitWriter &writer;

	/*
	** Handles the logic behind the probability model.
	*/
	BinaryModel model;

	/*
	** Tracks the number of pending bits.
	*/
	int pendingBitCounter;


private:
	/*
	** Writes a single bit to the output stream.
	*/
	void writeBit(bit_t);

	/*
	** Adds a pending bit to the pending bit counter.
	*/
	void addPendingBit();

	/*
	** Writes the pending bits to the output stream and resets the pending bit
	**   counter.
	*/
	void writePendingBits(bit_t);


public:
	/*
	** Constructor.
	*/
	BinaryEncoder(BitReader&, BitWriter&);

	/*
	** Main method to run the encoder.
	*/
	void run();
};
