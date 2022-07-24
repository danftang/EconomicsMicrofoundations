//
// Created by daniel on 21/07/22.
//

#include <random>
#include "Agent.h"
#include "Random.h"
#include "Simulation.h"

//void Agent::step(Simulation &sim) {
//    switch(status()) {
//        case ENTREPRENEUR:
//            spend(sim);
//            work(sim);
//            manageBusiness(sim);
//            break;
//
//        case UNEMPLOYED:
//            spend(sim);
//            wageExpectation = Random::nextDouble(0.0, wageExpectation);
//            negotiateEmployment(sim);
//            break;
//
//        case EMPLOYED:
//            spend(sim);
//            work(sim);
//            negotiateEmployment(sim);
//            break;
//    }
//}


void Agent::step(Simulation &sim) {
    if(status() != ENTREPRENEUR) negotiateEmployment(sim);
    spend(sim);
    if(status() != UNEMPLOYED) work(sim);
    if(status() == ENTREPRENEUR) manageBusiness(sim);
    if(status() != EMPLOYED) {
        sim.setHiringProbability(*this, wealth);
    } else {
        sim.setHiringProbability(*employer, employer->wealth);
    }
}


void Agent::negotiateEmployment(Simulation &sim) {
    Agent &employer =  sim.choosePotentialEmployer(*this);
    if(&employer != this) {
//        if(status() == UNEMPLOYED) wageExpectation = Random::nextDouble(0.0, wageExpectation);
        double negotiatedWage = Random::nextDouble(wageExpectation, 2.0 * wageExpectation);
        if(negotiatedWage > employer.wealth) negotiatedWage = employer.wealth;
        if(negotiatedWage > wageExpectation) {
            assert(status() != ENTREPRENEUR);
            assert(employer.status() != EMPLOYED);
            assert(employees.size() == 0);
            if (status() == Agent::UNEMPLOYED) {
                sim.setHiringProbability(*this, 0.0);
            } else {
                this->employer->employees.remove(this);
            }
            this->employer = &employer;
            employer.employees.push_back(this);
            wageExpectation = negotiatedWage;
        }
        else if(status() == UNEMPLOYED) {
            wageExpectation = Random::nextDouble(0.0, wageExpectation);
        }
    }
}

void Agent::spend(Simulation &sim) {
    double spending = Random::nextDouble(0.0, wealth);
    wealth -= spending;
    sim.aggregateDemand += spending;
}

void Agent::work(Simulation &sim) {
    double revenueGeneration = Random::nextDouble(0.0, sim.aggregateDemand);
    employer->wealth += revenueGeneration;
    sim.aggregateDemand -= revenueGeneration;
}

void Agent::manageBusiness(Simulation &sim) {
    for(auto employeeIt = employees.begin(); employeeIt != employees.end(); ++employeeIt) {
        assert(*employeeIt != this);
        assert((*employeeIt)->employer == this);
        if (wealth >= (*employeeIt)->wageExpectation) {
            (*employeeIt)->wealth += (*employeeIt)->wageExpectation;
            (*employeeIt)->lastWage = (*employeeIt)->wageExpectation;
            wealth -= (*employeeIt)->wageExpectation;
        } else { // fire employee
            (*employeeIt)->employer = *employeeIt;
            (*employeeIt)->wageExpectation = Random::nextDouble(0.0,(*employeeIt)->lastWage); // DIFFERENT FROM PAPER
//            (*employeeIt)->wageExpectation = (*employeeIt)->lastWage;
            sim.setHiringProbability(**employeeIt, (*employeeIt)->wealth);
            employeeIt = employees.erase(employeeIt);
            --employeeIt;
        }
    }
    if(employees.size() == 0) wageExpectation = wealth;
}


