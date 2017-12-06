#pragma once

#include "base/fixed.h"

class RateController {
public:
	/*
	** Data types.
	*/
	typedef Fixed<20> fixed;

	/*
	** Gets the number of bits alloted to the next symbol.
	*/
	virtual fixed symbolBudget() const = 0;

	/*
	** Informs the rate controller how many bits were used by the prior symbol.
	*/
	virtual void spendBits(fixed bits = 0) = 0;
};
