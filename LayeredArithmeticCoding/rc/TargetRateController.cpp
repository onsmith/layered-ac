#include "TargetRateController.h"


TargetRateController::TargetRateController(double target, double budget) :
	target(target),
	budget(budget) {
	spendBits();
}

double TargetRateController::symbolBudget() const {
	return budget;
}

void TargetRateController::spendBits(double cost) {
	budget += target - cost;
}
