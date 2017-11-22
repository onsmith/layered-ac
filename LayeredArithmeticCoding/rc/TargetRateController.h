#pragma once

#include "RateController.h"


class TargetRateController : public RateController {
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
	** Gets the number of bits alloted to the next symbol.
	*/
	double symbolBudget() const final;

	/*
	** Informs the rate controller how many bits were used by the prior symbol.
	*/
	void spendBits(double bits = 0.0) final;
};
