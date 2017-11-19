#pragma once

#include <iostream>
using std::istream;


class BitReader {
private:
	/*
	** Holds the input stream from which this object will read.
	*/
	istream& stream;

	/*
	** Buffers bits into byte-sized chunks (hah).
	*/
	char buffer;

	/*
	** Keeps track of the last read bit position in the buffer byte.
	*/
	unsigned char mask;


private:
	/*
	** Reads the next byte from the input stream into the buffer and resets the
	**   buffer.
	*/
	void fillBuffer();


public:
	/*
	** Constructor.
	*/
	BitReader(istream&);

	/*
	** Reads the next buffered bit from the istream.
	*/
	bool readBit();

	/*
	** Returns true iff there are no more bits to get in the stream.
	*/
	bool eof() const;
};
