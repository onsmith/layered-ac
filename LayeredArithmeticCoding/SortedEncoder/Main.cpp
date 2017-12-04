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

#include <vector>
using std::vector;

#include "SortedEncoder.h"

#include "base/Fixed.h"
#include "io/BitReader.h"
#include "io/BitWriter.h"
#include "model/ByteModel.h"
#include "rc/TargetRateController.h"
#include "base/RateDropArithmeticEncoder.h"


/*
** Encoding parameters.
*/
typedef unsigned char Symbol;
typedef Fixed<false, 20> fixed;
static const fixed BITS_PER_SYMBOL(1.5);
static const fixed INITIAL_BIT_SURPLUS(0);


/*
** Prints the program usage to the given output stream.
*/
void print_usage(ostream& stream, char* program_filename) {
	stream << "Usage: "
	       << program_filename
	       << " input-filename output-filename-1 [output-filename-2 ...]"
	       << endl;
}


/*
** Defines the entry point for the console application.
*/
int main(int argc, char *argv[]) {
	// Check number of passed command line arguments
	if (argc < 3) {
		cerr << "Incorrect number of arguments." << endl;
		print_usage(cerr, argv[0]);
		getchar();
		return 1;
	}

	// Parse command line arguments
	const char  *program_filename     = argv[0];
	const char  *input_filename       = argv[1];
	      char **output_filenames     = &argv[2];
	const int    num_output_filenames = argc - 2;

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

	// Open output files
	vector<ofstream> output_files(num_output_filenames);
	for (int i = 0; i < output_files.size(); i++) {
		output_files[i].open(output_filenames[i], ios::out | ios::binary | ios::trunc);
		if (!output_files[i].is_open()) {
			cerr << "Could not open file: "
					 << output_filenames[i]
					 << endl;
			getchar();
			return 1;
		}
	}

	// Prepare arithmetic encoder
	ByteModel model;
	vector<BitWriter*> bit_writers;
	vector<TargetRateController*> rate_controllers;
	vector<RateDropArithmeticEncoder<Symbol>> layers;
	for (int i = 0; i < output_files.size(); i++) {
		bit_writers.push_back(new BitWriter(output_files[i]));
		rate_controllers.push_back(new TargetRateController(BITS_PER_SYMBOL, INITIAL_BIT_SURPLUS));
		layers.emplace_back(*bit_writers[i], model, *rate_controllers[i]);
	}
	SortedEncoder<Symbol> encoder(layers);

	// Run arithmetic encoder
	while (true) {
		Symbol symbol;
		input_file.read(reinterpret_cast<char*>(&symbol), sizeof(symbol));
		if (input_file.eof()) { break; }
		encoder.encode(symbol);
	}
	encoder.finish();

	// Close files
	input_file.close();
	for (int i = 0; i < output_files.size(); i++) {
		output_files[i].close();
	}

	// Free memory
	for (int i = 0; i < layers.size(); i++) {
		delete bit_writers[i];
		delete rate_controllers[i];
	}

	// All done
	cout << endl << "Finished." << endl;
	getchar();
	return 0;
}
