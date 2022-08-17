//
// Created by daniel on 21/07/22.
//

#include <random>
#include "Person.h"
#include "mystd/Random.h"
#include "Simulation.h"

Person::Person() {
    mu = {1.0 , 0.5, 0.0};
    sigma = {0.25, 0.25, initialAgentWealth};
    employer = nullptr; // unemployed
    wageExpectation = 1;
    lastWage = 1;
    currentConsumptionWellbeing = 0.0;
    sim.bank.transfer(sim.bank.reserveAccount, bankAccount, initialAgentWealth);
}


void Person::step() {
    if(Random::nextBernoulli(1.0/(47.0*12.0))) die();
    negotiateEmployment();
    spend();
//    if(isEmployed()) work(sim);
}


void Person::negotiateEmployment() {
    double rand = Random::nextDouble();
    double pStartNewCompany = 0.025;

    // rational based on toilWellbeing and restlessness if toil is non-optimal
    double cLookForNewJob = isEmployed()?pStartNewCompany + 0.05 * (2.0 - toilWellbeing()):1.0;

    // this function creates a split product marketplace, if a company holds on to employees it
    // can somehow carry on (cheaper product for lower paid?)
//    double cLookForNewJob = isEmployed()?pStartNewCompany + 0.05 * (employer->toilPerUnitproduct - 0.5):1.0;

    if(rand < pStartNewCompany) {
        Company *newCompany = sim.startNewCompany(wageExpectation/2+1, Random::nextDouble(), Random::nextDouble(0.5, 1.5));
        if(newCompany != nullptr) {
            if(isEmployed()) employer->endEmployment(*this);
            wageExpectation = wageExpectation/2+1;
            newCompany->hire(this);
        }
    } else if(rand < cLookForNewJob) {
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
    Company *bestCompany = selectProductFromAdvertising();
    if(bestCompany != nullptr) {
        sim.bank.transfer(bankAccount, bestCompany->bankAccount, bestCompany->unitPrice);
        sim.cumulativeDemand += bestCompany->unitPrice;
        bestCompany->stock -= 1.0;
        currentConsumptionWellbeing = consumptionWellbeing(bestCompany->product);
    } else {
        currentConsumptionWellbeing = 0.0;
    }
}

// Die/retire and be replaced by offspring
// offspring inherits all wealth
void Person::die() {
    if(isEmployed()) employer->endEmployment(*this);
    wageExpectation = 1;
    lastWage = 1;
}

//double Person::wellbeing(Person::WellbeingDimension d, double x) const {
//    double z = (x - mu[d])/sigma[d];
//    return exp(-(z*z)); // not normalised since we want to standardise the maximum, not the integral (wellbeing isn't a PDF)
//}

double Person::wellbeing() const {
    return currentConsumptionWellbeing * toilWellbeing() * securityWellbeing();
}

double Person::toilWellbeing() const {
    if(!isEmployed()) return 0.0;
    double x = (employer->toilPerUnitproduct-mu[TOIL])/sigma[TOIL];
    return exp(-x*x);
}

double Person::securityWellbeing() const { return securityWellbeing(bankAccount->balance()); }

double Person::securityWellbeing(double balance) const {
    double x = balance/sigma[TOIL];
    return erf(x);
}

// find best product out of 20 draws by company wealth
// may return null if can't afford any found product
Company *Person::selectProductFromAdvertising() {
    const int nProductsToSearch = 20;
    Company *bestCompany = nullptr;
    double bestNonToilWellbeing = 0.0;
    for(int t=0; t<nProductsToSearch; ++t) {
        Company &manufacturer = *sim.chooseEmployerByWealth();
        if(manufacturer.stock >= 1.0 && manufacturer.unitPrice <= bankAccount->balance()) {
            double potentialNonToilWellbeing = consumptionWellbeing(manufacturer.product)*securityWellbeing(bankAccount->balance() - manufacturer.unitPrice);
            if(potentialNonToilWellbeing > bestNonToilWellbeing) {
                bestCompany = &manufacturer;
                bestNonToilWellbeing = potentialNonToilWellbeing;
            }
        }
    }
    return bestCompany;
}

// find best product, exhaustive search
// may return null if can't afford any product
Company *Person::selectBestAvailableProduct() {
    Company *bestCompany = nullptr;
    double bestNonToilWellbeing = 0.0;
    for(auto manufacturerIt = sim.companies.begin(); manufacturerIt != sim.companies.end(); ++ manufacturerIt) {
        if(manufacturerIt->stock >= 1.0 && manufacturerIt->unitPrice <= bankAccount->balance()) {
            double potentialNonToilWellbeing = consumptionWellbeing(manufacturerIt->product)*securityWellbeing(bankAccount->balance() - manufacturerIt->unitPrice);
            if(potentialNonToilWellbeing > bestNonToilWellbeing) {
                bestCompany = &*manufacturerIt;
                bestNonToilWellbeing = potentialNonToilWellbeing;
            }
        }
    }
    return bestCompany;
}

