#pragma once

#include "RateController.h"

#include <cstdint>


class TargetRateController : public RateController {
private:
	/*
	** The number of bits allocated for each symbol.
	*/
	fixed target;

	/*
	** The current bit budget.
	*/
	fixed budget;


public:
	/*
	** Constructors.
	*/
	TargetRateController(fixed target, fixed surplus = 0);

	/*
	** Gets the number of bits alloted to the next symbol.
	*/
	fixed symbolBudget() const final;

	/*
	** Informs the rate controller how many bits were used by the prior symbol.
	*/
	void spendBits(fixed bits = 0) final;
};
