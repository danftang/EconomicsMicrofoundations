//
// Created by daniel on 21/07/22.
//

#include <random>
#include "Person.h"
#include "mystd/Random.h"
#include "Simulation.h"


void Person::step(Simulation &sim) {
    negotiateEmployment(sim);
    spend(sim);
    if(isEmployed()) work(sim);
    invest(sim);
}


void Person::negotiateEmployment(Simulation &sim) {
    double pStartNewCompany = 0.25;// sim.fund.wealth / (sim.fund.wealth + sim.companies.sum());
    if(Random::nextDouble() < pStartNewCompany) {
        Company *newCompany = sim.startNewCompany(wageExpectation);
        if(newCompany != nullptr) startWorkingFor(newCompany);
    } else {
        Company &newEmployer = sim.choosePotentialEmployer();
        double negotiatedWage = newEmployer.negotiateWage(wageExpectation);
        if (negotiatedWage > wageExpectation) {
            startWorkingFor(&newEmployer);
            wageExpectation = negotiatedWage;
        } else if (!isEmployed()) {
            wageExpectation = Random::nextDouble(0.0, wageExpectation);
        }
    }
}


void Person::spend(Simulation &sim) {
    double spending = Random::nextDouble(0.0, wealth);
    transferMoneyTo(sim.aggregateDemandAccount, spending);
}


void Person::work(Simulation &sim) {
    double valueOfWork = Random::nextDouble(0.0, sim.aggregateDemandAccount.wealth);
    sim.aggregateDemandAccount.transferMoneyTo(*employer, valueOfWork);
}


void Person::invest(Simulation &sim) {
    double investment = Random::nextDouble(0.0, wealth);
    wealth -= investment;
    sim.fund.wealth += investment;
}

void Person::startWorkingFor(Company *newEmployer) {
    if(employer != nullptr) employer->endEmployment(this);
    employer = newEmployer;
    newEmployer->employees.push_back(this);
}
