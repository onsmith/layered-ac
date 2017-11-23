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

#include "RateDropArithmeticDecoder.h"

#include "io/BitReader.h"
#include "io/BitWriter.h"
#include "model/ByteModel.h"
#include "model/BinaryModel.h"
#include "rc/TargetRateController.h"


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
	ByteModel model;
	TargetRateController rateController(4, 100*8); // target bits per symbol, initial bit surplus
	RateDropArithmeticDecoder<unsigned char> decoder(reader, model, rateController);

	// Run arithmetic decoder
	double decoded = 0.0;
	double dropped = 0.0;
	while (!reader.eof()) {
		double max_cost = model.getSubrange(model.getCostliestSymbol()).bitcost();
		double budget = rateController.symbolBudget();
		//cout << "Need at most " << max_cost << " bits; have " << budget << " bits." << endl;
		if (decoder.canDecodeNextSymbol()) {
			auto symbol = decoder.decode();
			output_file.write(reinterpret_cast<char*>(&symbol), sizeof(symbol));
			//cout << "Decoded " << symbol << " (" << static_cast<int>(symbol) << ")." << endl;
			decoded++;
		} else {
			decoder.skipSymbol();
			auto symbol = 'X'; //model.getCheapestSymbol();
			output_file.write(reinterpret_cast<char*>(&symbol), sizeof(symbol));
			//cout << "Couldn't decode next symbol. Chose " << symbol << " instead." << endl;
			dropped++;
		}
		//getchar();
	}

	// All done
	cout << endl << "Finished." << endl;
	cout << "Decoded " << decoded << " symbols; dropped " << dropped << " symbols." << endl;
	getchar();
	return 0;
}
