#include "TargetRateController.h"


TargetRateController::TargetRateController(fixed target, fixed surplus) :
	target(target),
	budget(surplus) {
	spendBits();
}

TargetRateController::fixed TargetRateController::symbolBudget() const {
	return budget;
}

void TargetRateController::spendBits(fixed cost) {
	budget += target - cost;
}
