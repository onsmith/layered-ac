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

#include "RateBinaryArithmeticDecoder.h"

#include "io/BitReader.h"
#include "io/BitWriter.h"
#include "model/BinaryModel.h"


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
	RateBinaryArithmeticDecoder decoder(reader, model, 0.4, 5.0);

	// Run arithmetic decoder
	double decoded = 0.0;
	double dropped = 0.0;
	double spent   = 0.0;
	while (!reader.eof()) {
		double max_cost = model.getSubrange(model.getCostliestSymbol()).bitcost();
		double budget = decoder.nextSymbolBudget();
		cout << "Need at most " << max_cost << " bits; have " << budget << " bits." << endl;
		if (max_cost <= budget) {
			bool symbol = decoder.decode();
			double actual_cost = model.getSubrange(symbol).bitcost();
			spent += actual_cost;
			model.observe(symbol);
			writer.writeBit(symbol);
			cout << "Decoded " << (symbol ? '1' : '0') << ", spending " << actual_cost << " bits." << endl;
			decoded++;
		} else {
			bool symbol = model.getCheapestSymbol();
			writer.writeBit(symbol);
			decoder.spendBits(0.0);
			cout << "Couldn't decode next symbol. Chose " << (symbol ? '1' : '0') << " instead." << endl;
			dropped++;
		}
		//getchar();
	}

	// All done
	cout << endl << "Finished." << endl;
	cout << "Decoded " << decoded << " symbols; dropped " << dropped << " symbols." << endl;
	cout << "Output compression ratio is " << spent / (decoded + dropped) << endl;
	getchar();
	return 0;
}
