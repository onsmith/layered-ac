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

#include <cmath>

#include "io/BitReader.h"
#include "io/BitWriter.h"
#include "model/ByteModel.h"
#include "rc/TargetRateController.h"
#include "base/RateDropArithmeticEncoder.h"


/*
** Encoding parameters.
*/
#define BITS_PER_SYMBOL 8
#define INITIAL_BIT_SURPLUS 800
typedef unsigned char Symbol;


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
	BitWriter writer(output_file);
	ByteModel model;
	TargetRateController rateController(BITS_PER_SYMBOL, INITIAL_BIT_SURPLUS);
	RateDropArithmeticEncoder<Symbol> encoder(writer, model, rateController);

	// Run arithmetic encoder
	double encoded = 0.0;
	double dropped = 0.0;
	double spent   = 0.0;
	while (true) {
		Symbol symbol;
		input_file.read(reinterpret_cast<char*>(&symbol), sizeof(symbol));
		if (input_file.eof()) { break; }
		double actual_cost = model.getSubrange(symbol).bitcost();
		double max_cost    = model.getSubrange(model.getCostliestSymbol()).bitcost();
		double budget      = rateController.symbolBudget();
		//cout << "Need at most " << max_cost << " bits; have " << budget << " bits." << endl;
		if (encoder.canEncodeNextSymbol()) {
			//cout << "Encoded " << symbol << ", spending " << actual_cost << " bits." << endl;
			encoder.encode(symbol);
			model.update(symbol);
			spent += actual_cost;
			encoded++;
		} else {
			//cout << "Couldn't encode " << symbol << '.' << endl;
			encoder.skipSymbol();
			dropped++;
		}
		//getchar();
	}
	encoder.finish();

	// All done
	cout << endl << "Finished." << endl;
	cout << "Encoded " << encoded << " symbols; dropped " << dropped << " symbols." << endl;
	cout << "Output compression ratio is " << spent / (encoded + dropped) << endl;
	getchar();
	return 0;
}
