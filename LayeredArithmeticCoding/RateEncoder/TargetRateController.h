#pragma once


class TargetRateController {
private:
	/*
	** The number of bits allocated for each symbol.
	*/
	double target;

	/*
	** The current bit budget.
	*/
	double budget;


public:
	/*
	** Constructors.
	*/
	TargetRateController(double target, double budget = 0.0);

	/*
	** Gets the number of bits (as a Rational) alloted to the next symbol.
	*/
	double nextSymbolBudget();

	/*
	** Informs the rate controller how many bits were used by the prior symbol.
	*/
	void spendBits(double);

	/*
	** Virtual destructor.
	*/
	virtual ~TargetRateController() = default;
};
