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
#include "ArithmeticDecoder.h"

#include "io/BitReader.h"
#include "io/BitWriter.h"


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
	ArithmeticDecoder<bool> decoder(reader, model);

	// Run arithmetic decoder
	double bits_consumed  = 0;
	double bits_outputted = 0;
	while (true) {
		bool bit = decoder.decode();
		if (reader.eof()) { break; }
		bits_outputted++;
		auto range = model.getSubrange(bit);
		auto cost = log2(static_cast<double>(range.range) / (range.high - range.low));
		bits_consumed += cost;
		model.observe(bit);
		writer.writeBit(bit);
		cout << "Decoded " << (bit ? '1' : '0') << ", consuming " << cost << " bits." << endl;
	}

	// All done
	cout << endl << "Finished." << endl;
	cout << "Consumed " << (bits_consumed / 8) << " bytes to decode " << (bits_outputted / 8) << " bytes." << endl;
	getchar();
	return 0;
}
