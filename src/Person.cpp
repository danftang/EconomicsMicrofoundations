//
// Created by daniel on 21/07/22.
//

#include <random>
#include "Person.h"
#include "mystd/Random.h"
#include "Simulation.h"


void Person::step() {
    if(Random::nextBernoulli(1.0/(47.0*12.0))) die();
    negotiateEmployment();
    spend();
//    if(isEmployed()) work(sim);
}


void Person::negotiateEmployment() {
    double pStartNewCompany = 0.1;
    if(Random::nextDouble() < pStartNewCompany) {
        Company *newCompany = sim.startNewCompany(wageExpectation/2+1, Random::nextDouble());
        if(newCompany != nullptr) {
            if(isEmployed()) employer->endEmployment(*this);
            wageExpectation = wageExpectation/2+1;
            newCompany->hire(this);
        }
    } else {
        auto newEmployer = sim.chooseEmployerByWealth();
        if(newEmployer != sim.companies.end()) {
            int negotiatedWage = newEmployer->negotiateWage(*this);
            if (negotiatedWage > wageExpectation) {
                if(isEmployed()) employer->endEmployment(*this);
                wageExpectation = negotiatedWage;
                newEmployer->hire(this);
            }
        }
    }
    if (!isEmployed()) wageExpectation = Random::nextInt(1, wageExpectation + 1);
}


void Person::spend() {
    if(sim.companies.size() == 0) return;
    const int nProductsToSearch = 10;
    Company *bestCompany = nullptr;
    consumptionWellbeing = 0.0;
    for(int t=0; t<nProductsToSearch; ++t) {
        Company &manufacturer = *sim.chooseEmployerByWealth();
        if(manufacturer.stock >= 1.0 && manufacturer.unitPrice <= bankAccount->balance()) {
            double potentialConsumptionWellbeing = wellbeing(CONSUMPTION, manufacturer.product);
            if(potentialConsumptionWellbeing > consumptionWellbeing) {
                bestCompany = &manufacturer;
                consumptionWellbeing = potentialConsumptionWellbeing;
            }
        }
    }
    if(bestCompany != nullptr) {
        sim.bank.transfer(bankAccount, bestCompany->bankAccount, bestCompany->unitPrice);
        sim.cumulativeDemand += bestCompany->unitPrice;
        bestCompany->stock -= 1.0;
    }
}


//void Person::work(Simulation &sim) {
//    int valueOfWork = Random::nextInt(0, sim.aggregateDemandAccount->balance() + 1);
//    sim.bank.transfer(sim.aggregateDemandAccount, employer->bankAccount, valueOfWork);
//}


// Die/retire and be replaced by offspring
// offspring inherits all wealth
void Person::die() {
    if(isEmployed()) employer->endEmployment(*this);
    wageExpectation = 1;
    lastWage = 1;
}

double Person::wellbeing(Person::WellbeingDimension d, double x) const {
    double z = (x - mu[d])/sigma[d];
    return exp(-(z*z)); // not normalised since we want to standardise the maximum, not the integral (wellbeing isn't a PDF)
}

double Person::wellbeing() const {
    return wellbeing(TOIL, isEmployed()?employer->productivityPerEmployee:0.0) * consumptionWellbeing;
}
