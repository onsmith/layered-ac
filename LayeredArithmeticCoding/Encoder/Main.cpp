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
#include "base/ArithmeticEncoder.h"

#include "io/BitReader.h"
#include "io/BitWriter.h"


/*
** Encoding parameters.
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
	ArithmeticEncoder<Symbol> encoder(writer, model);

	// Run arithmetic encoder
	double symbols_encoded = 0;
	double bits_outputted = 0;
	while (true) {
		Symbol symbol = reader.readBit();
		if (reader.eof()) { break; }
		symbols_encoded++;
		double cost = model.getSubrange(symbol).bitcost();
		encoder.encode(symbol);
		model.update(symbol);
		bits_outputted += cost;
		cout << "Encoded " << (symbol ? '1' : '0') << " spending " << cost << " bits." << endl;
	}
	encoder.finish();

	// All done
	cout << endl << "Finished." << endl;
	cout << "Spent " << (bits_outputted / 8) << " bytes to encode " << (symbols_encoded / 8) << " bytes." << endl;
	getchar();
	return 0;
}
