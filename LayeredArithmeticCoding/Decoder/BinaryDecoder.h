#pragma once

#include "io/BitReader.h"
#include "io/BitWriter.h"

#include "Encoder/BinaryModel.h"


class BinaryDecoder {
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


public:
	/*
	** Constructor.
	*/
	BinaryDecoder(BitReader&, BitWriter&);

	/*
	** Main method to run the decoder.
	*/
	void run();
};
