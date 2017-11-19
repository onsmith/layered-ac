#include "BitReader.h"


BitReader::BitReader(istream &stream) :
	stream(stream),
	buffer(0),
	mask(0) {
}

bool BitReader::readBit() {
	mask >>= 1;
	if (mask == 0) {
		fillBuffer();
	}
	return (buffer & mask) != 0;
}

bool BitReader::eof() const {
	return stream.eof() && mask == 0;
}

void BitReader::fillBuffer() {
	stream.read(&buffer, sizeof(buffer));
	if (!stream.eof()) {
		mask = 0x80;
	}
}
