//
// Created by daniel on 21/07/22.
//

#include <random>
#include "Agent.h"
#include "Random.h"

void Agent::negotiateEmployment(Agent &employer) {
    double negotiatedWage = Random::nextDouble(wageExpectation, 2.0*wageExpectation);
    if(negotiatedWage <= employer.wealth) {
        this->employer = &employer;
        employer.employees.push_back(this);
        wageExpectation = negotiatedWage;
    }
}

double Agent::spend() {
    double spending = Random::nextDouble(0.0, wealth);
    wealth -= spending;
    return spending;
}

double Agent::work(double aggregateDemand) {
    if(status() != Agent::UNEMPLOYED) {
        double revenueGeneration = Random::nextDouble(0.0, aggregateDemand);
        if(status() == Agent::EMPLOYED) {
            employer->wealth += revenueGeneration;
        } else {
            wealth += revenueGeneration;
        }
        return revenueGeneration;
    }
    return 0.0;
}

void Agent::manageBusiness() {
    for(auto employee = employees.begin(); employee != employees.end(); ++employee) {
        if(wealth >= (*employee)->wageExpectation) {
            (*employee)->wealth += (*employee)->wageExpectation;
            wealth -= (*employee)->wageExpectation;
        } else { // fire employee
            (*employee)->employer = nullptr;
            employee = employees.erase(employee);
            --employee;
        }
    }
}


