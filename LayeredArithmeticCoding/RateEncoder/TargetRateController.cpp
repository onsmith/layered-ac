#include "TargetRateController.h"


TargetRateController::TargetRateController(double target, double budget) :
	target(target),
	budget(budget) {
	spendBits(0.0);
}

double TargetRateController::nextSymbolBudget() {
	return budget;
}

void TargetRateController::spendBits(double cost) {
	budget += target - cost;
}
