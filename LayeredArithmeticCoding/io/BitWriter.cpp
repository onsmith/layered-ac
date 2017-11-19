#include "BitWriter.h"


BitWriter::BitWriter(ostream &stream) :
  stream(stream),
  buffer(0),
  mask(0x80) {
}

BitWriter::~BitWriter() {
	if (mask != 0x80) {
		flushBuffer();
	}
}

void BitWriter::writeBit(bool bit) {
	if (bit) {
		buffer |= mask;
	}
	mask >>= 1;
	if (mask == 0) {
		flushBuffer();
	}
}

void BitWriter::flushBuffer() {
	stream.write(&buffer, sizeof(buffer));
	buffer = 0;
	mask = 0x80;
}
