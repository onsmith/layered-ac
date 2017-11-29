#include <cstdint>

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
using std::ostream;

#include <fstream>
using std::ifstream;
using std::ofstream;
using std::ios;

#include "model/BinaryModel.h"
#include "base/ArithmeticDecoder.h"

#include "io/BitReader.h"
#include "io/BitWriter.h"


/*
** Decoding parameters.
*/
typedef bool Symbol;


/*
** Prints the program usage to the given output stream.
*/
void print_usage(ostream& stream, char* program_filename) {
	stream << "Usage: "
	       << program_filename
	       << " input-filename output-filename"
	       << endl;
}


/*
** Defines the entry point for the console application.
*/
int main(int argc, char *argv[]) {
	// Check number of passed command line arguments
	if (argc != 3) {
		cerr << "Incorrect number of arguments." << endl;
		print_usage(cerr, argv[0]);
		getchar();
		return 1;
	}

	// Parse command line arguments
	const char *program_filename = argv[0];
	const char *input_filename   = argv[1];
	const char *output_filename  = argv[2];

	// Open input file
	ifstream input_file;
	input_file.open(input_filename, ios::in | ios::binary);
	if (!input_file.is_open()) {
		cerr << "Could not open file: "
		     << input_filename
		     << endl;
		getchar();
		return 1;
	}

	// Open output file
	ofstream output_file;
	output_file.open(output_filename, ios::out | ios::binary | ios::trunc);
	if (!output_file.is_open()) {
		cerr << "Could not open file: "
		     << output_filename
		     << endl;
		getchar();
		return 1;
	}

	// Prepare arithmetic encoder
	BitReader reader(input_file);
	BitWriter writer(output_file);
	BinaryModel model;
	ArithmeticDecoder<Symbol> decoder(reader, model);

	// Run arithmetic decoder
	double bits_consumed = 0;
	double symbols_outputted = 0;
	while (true) {
		Symbol symbol = decoder.decode();
		if (decoder.eof()) { break; }
		symbols_outputted++;
		double cost = model.getSubrange(symbol).bitcost();
		bits_consumed += cost;
		model.update(symbol);
		writer.writeBit(symbol);
		cout << "Decoded " << (symbol ? '1' : '0') << ", consuming " << cost << " bits." << endl;
	}

	// All done
	cout << endl << "Finished." << endl;
	cout << "Consumed " << (bits_consumed / 8) << " bytes to decode " << (symbols_outputted / 8) << " bytes." << endl;
	getchar();
	return 0;
}
