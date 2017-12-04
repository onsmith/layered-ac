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

#include "base/Fixed.h"
#include "io/BitReader.h"
#include "io/BitWriter.h"
#include "model/ByteModel.h"
#include "model/ProbabilityRange.h"
#include "rc/TargetRateController.h"
#include "base/RateDropArithmeticEncoder.h"


/*
** Encoding parameters.
*/
typedef unsigned char Symbol;
typedef Fixed<false, 20> fixed;
static const fixed BITS_PER_SYMBOL(4);
static const fixed INITIAL_BIT_SURPLUS(0);


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
	int encoded = 0;
	int dropped = 0;
	fixed spent = 0.0;
	while (true) {
		Symbol symbol;
		input_file.read(reinterpret_cast<char*>(&symbol), sizeof(symbol));
		if (input_file.eof()) { break; }
		fixed actual_cost = model.getSubrange(symbol).bitcost();
		fixed max_cost    = model.getSubrange(model.getCostliestSymbol()).bitcost();
		fixed budget      = rateController.symbolBudget();
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
	cout << "Processed " << (encoded + dropped) << " symbols in total." << endl;
	cout << "Encoded " << encoded << " symbols." << endl;
	cout << "Dropped " << dropped << " symbols." << endl;
	cout << "Encoded " << fixed(encoded) / (encoded + dropped) * 100 << "% of symbols." << endl;
	cout << "Dropped " << fixed(dropped) / (encoded + dropped) * 100 << "% of symbols." << endl;
	cout << "Processed " << (encoded + dropped) << " bytes." << endl;
	cout << "Output " << spent / 8 << " bytes." << endl;
	cout << "Output " << spent / (encoded + dropped) << " bits per symbol." << endl;
	cout << "Attained a compression ratio of " << fixed(8 * (encoded + dropped)) / spent << endl;
	getchar();
	return 0;
}
