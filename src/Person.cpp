//
// Created by daniel on 21/07/22.
//

#include <random>
#include "Person.h"
#include "mystd/Random.h"
#include "Simulation.h"


void Person::step(Simulation &sim) {
    if(Random::nextBernoulli(1.0/(47.0*12.0))) die(sim);
    negotiateEmployment(sim);
    spend(sim);
    if(isEmployed()) work(sim);
}


void Person::negotiateEmployment(Simulation &sim) {
    double pStartNewCompany = 0.1;
    if(Random::nextDouble() < pStartNewCompany) {
        Company *newCompany = sim.startNewCompany(wageExpectation/2+1);
        if(newCompany != nullptr) {
            if(isEmployed()) employer->endEmployment(this);
            wageExpectation = wageExpectation/2+1;
            newCompany->hire(this);
        }
    } else {
        auto newEmployer = sim.choosePotentialEmployer();
        if(newEmployer != sim.companies.end()) {
            int negotiatedWage = newEmployer->negotiateWage(wageExpectation);
            if (negotiatedWage > wageExpectation) {
                if(isEmployed()) employer->endEmployment(this);
                wageExpectation = negotiatedWage;
                newEmployer->hire(this);
            }
        }
    }
    if (!isEmployed()) wageExpectation = Random::nextInt(1, wageExpectation + 1);
}


void Person::spend(Simulation &sim) {
    int spending = Random::nextInt(0, bankAccount->balance() + 1);
    sim.bank.transfer(bankAccount, sim.aggregateDemandAccount, spending);
    sim.cumulativeDemand += spending;
}


void Person::work(Simulation &sim) {
    int valueOfWork = Random::nextInt(0, sim.aggregateDemandAccount->balance() + 1);
    sim.bank.transfer(sim.aggregateDemandAccount, employer->bankAccount, valueOfWork);
}


// Die/retire and be replaced by offspring
// offspring inherits all wealth
void Person::die(Simulation &sim) {
    if(isEmployed()) employer->endEmployment(this);
    wageExpectation = 1;
    lastWage = 1;
}
