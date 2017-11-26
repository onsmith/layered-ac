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

#include "SortedDecoder.h"

#include "io/BitReader.h"
#include "io/BitWriter.h"
#include "model/ByteModel.h"
#include "rc/TargetRateController.h"
#include "base/RateDropArithmeticDecoder.h"


/*
** Prints the program usage to the given output stream.
*/
void print_usage(ostream& stream, char* program_filename) {
	stream << "Usage: "
	       << program_filename
	       << " input-filename-1 [input-filename-2 ...] output-filename"
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
	const char  *program_filename    = argv[0];
	      char **input_filenames     = &argv[1];
	const char  *output_filename     = argv[argc - 1];
	const int    num_input_filenames = argc - 2;

	// Open input files
	vector<ifstream> input_files(num_input_filenames);
	for (int i = 0; i < input_files.size(); i++) {
		input_files[i].open(input_filenames[i], ios::in | ios::binary);
		if (!input_files[i].is_open()) {
			cerr << "Could not open file: "
					 << input_filenames[i]
					 << endl;
			getchar();
			return 1;
		}
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
	ByteModel model;
	vector<BitReader*> bit_readers;
	vector<TargetRateController*> rate_controllers;
	vector<RateDropArithmeticDecoder<unsigned char>> layers;
	for (int i = 0; i < input_files.size(); i++) {
		bit_readers.push_back(new BitReader(input_files[i]));
		rate_controllers.push_back(new TargetRateController(4.0, 100*8));
		layers.emplace_back(*bit_readers[i], model, *rate_controllers[i]);
	}
	SortedDecoder<unsigned char> decoder(layers);

	// Run arithmetic decoder
	while (!input_files[0].eof()) {
		auto symbol(decoder.decode());
		output_file.write(reinterpret_cast<char*>(&symbol), sizeof(symbol));
	}

	// All done
	cout << endl << "Finished." << endl;
	getchar();
	return 0;
}