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
    double cLookForNewJobIfEmployed = pStartNewCompany + 0.05;
    if(rand < pStartNewCompany) {
        Company *newCompany = sim.startNewCompany(wageExpectation/2+1, Random::nextDouble(), Random::nextDouble(0.5, 1.5));
        if(newCompany != nullptr) {
            if(isEmployed()) employer->endEmployment(*this);
            wageExpectation = wageExpectation/2+1;
            newCompany->hire(this);
        }
    } else if(!isEmployed() || rand < cLookForNewJobIfEmployed) {
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
    Company *bestCompany = nullptr;
    double bestNonToilWellbeing = 0.0;

//    // find best product. Advertising version
//    const int nProductsToSearch = 20;
//    for(int t=0; t<nProductsToSearch; ++t) {
//        Company &manufacturer = *sim.chooseEmployerByWealth();
//        if(manufacturer.stock >= 1.0 && manufacturer.unitPrice <= bankAccount->balance()) {
//            double potentialNonToilWellbeing = consumptionWellbeing(manufacturer.product)*securityWellbeing(bankAccount->balance() - manufacturer.unitPrice);
//            if(potentialNonToilWellbeing > bestNonToilWellbeing) {
//                bestCompany = &manufacturer;
//                bestNonToilWellbeing = potentialNonToilWellbeing;
//            }
//        }
//    }

    // find best product, exhaustive search
    for(auto manufacturerIt = sim.companies.begin(); manufacturerIt != sim.companies.end(); ++ manufacturerIt) {
        if(manufacturerIt->stock >= 1.0 && manufacturerIt->unitPrice <= bankAccount->balance()) {
            double potentialNonToilWellbeing = consumptionWellbeing(manufacturerIt->product)*securityWellbeing(bankAccount->balance() - manufacturerIt->unitPrice);
            if(potentialNonToilWellbeing > bestNonToilWellbeing) {
                bestCompany = &*manufacturerIt;
                bestNonToilWellbeing = potentialNonToilWellbeing;
            }
        }
    }


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
    return
        currentConsumptionWellbeing *
        toilWellbeing(isEmployed()?employer->productivityPerEmployee:0.0) *
        securityWellbeing(bankAccount->balance());
}

