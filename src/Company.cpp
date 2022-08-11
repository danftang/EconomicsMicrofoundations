//
// Created by daniel on 01/08/22.
//

#include <cassert>
#include "Company.h"
#include "Simulation.h"
#include "mystd/Random.h"

void Company::step(Simulation &sim) {
    auto employeeIt = employees.begin();
    while(employeeIt != employees.end()) {
        assert((*employeeIt)->employer == this);
        if (wealth >= (*employeeIt)->wageExpectation) {
            (*employeeIt)->wealth += (*employeeIt)->wageExpectation;
            (*employeeIt)->lastWage = (*employeeIt)->wageExpectation;
            wealth -= (*employeeIt)->wageExpectation;
            ++employeeIt;
        } else { // fire employee
            (*employeeIt)->employer = nullptr;
            (*employeeIt)->wageExpectation = Random::nextDouble(0.0,(*employeeIt)->lastWage); // DIFFERENT FROM PAPER, but needed to reproduce results
            employeeIt = employees.erase(employeeIt);
        }
    }
}

double Company::negotiateWage(double applicantWageExpectation) {
    double negotiatedWage = Random::nextDouble(applicantWageExpectation, 2.0 * applicantWageExpectation);
    if (negotiatedWage > wealth) negotiatedWage = wealth;
    return negotiatedWage;
}