#pragma once


template <typename Symbol>
class Encoder {
public:
	/*
	** Encodes a symbol.
	*/
	virtual void encode(Symbol) = 0;

	/*
	** Finishes encoding and flushes all pending output.
	*/
	virtual void finish() = 0;

	/*
	** Virtual destructor.
	*/
	virtual ~Encoder() = default;
};
