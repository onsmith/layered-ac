#pragma once

#include <iostream>
using std::ostream;


class BitWriter {
private:
	/*
	** Holds the output stream to which this object will write.
	*/
	ostream& stream;

	/*
	** Buffers bits into byte-sized chunks (hah).
	*/
	char buffer;

	/*
	** Keeps track of the next bit position in the buffer byte.
	*/
	unsigned char mask;


private:
	/*
	** Writes the buffer in its current state to the output stream and resets the
	**   buffer.
	*/
	void flushBuffer();


public:
	/*
	** Constructor.
	*/
	BitWriter(ostream&);

	/*
	** Destructor.
	*/
	~BitWriter();

	/*
	** Buffers a bit to be written to the ostream.
	*/
  void writeBit(bool);
};
