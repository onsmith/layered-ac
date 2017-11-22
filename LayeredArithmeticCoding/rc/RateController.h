#pragma once


class RateController {
public:
	/*
	** Gets the number of bits alloted to the next symbol.
	*/
	virtual double symbolBudget() const = 0;

	/*
	** Informs the rate controller how many bits were used by the prior symbol.
	*/
	virtual void spendBits(double bits = 0.0) = 0;
};
