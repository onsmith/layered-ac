#pragma once


template <typename Symbol>
class Decoder {
public:
	/*
	** Decodes a symbol.
	*/
	virtual Symbol decode() = 0;

	/*
	** Returns true iff the decoder has attempted to read past the end of the
	**   source data stream.
	*/
	virtual bool eof() const = 0;

	/*
	** Virtual destructor.
	*/
	virtual ~Decoder() = default;
};
