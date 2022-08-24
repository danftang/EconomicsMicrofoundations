//
// Created by daniel on 21/07/22.
//
// TODO: what happens if we remove the hiring bias towards large companies?
// ...or at least replace it by some measure of "apparent desirability"
// w.r.t. security, potential purchasing power, and perceived toil
// (without assuming people have access to their own wellbeing function
// i.e. they know their current wellbeing but cannot predict their
// wellbeing under hypothetical situations)
//
// TODO: Add natural ability to do various things, which can then be used as
// a basis for rational hiring and pay.


#include <random>
#include "Person.h"
#include "mystd/Random.h"
#include "Simulation.h"

Person::Person() {
    mu = {1.0, Random::nextGaussian(0.1, 0.05), 0.0};
//    mu = {1.0, 0.1, 0.0};
//    if(Random::nextBernoulli(0.5)) {
//        mu = {1.0, 0.1, 0.0};
//    } else {
//        mu = {1.0, 0.9, 0.0};
//    }
    sigma = {0.25, 0.25, 100.0};
    employer = nullptr; // unemployed
    wageExpectation = 100;
    lastWage = 100;
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
    double pStartNewCompany = 0.005;
    double restlessness = isEmployed()?0.1*(1.0-toilWellbeing()):1.0;
//    if(isEmployed()) std::cout << "restlessness = " << restlessness << " toilWellbieng = " << toilWellbeing() << " toil = " << employer->toilPerUnitproduct << std::endl;
    if(Random::nextBernoulli(restlessness)) {
//        std::cout << "isRestless" << std::endl;
        if(Random::nextBernoulli(pStartNewCompany)) {
            Company *newCompany = sim.startNewCompany(wageExpectation, Random::nextDouble(), Random::nextDouble(0.5, 1.5));
            if(newCompany != nullptr) {
                if(isEmployed()) employer->endEmployment(*this);
                newCompany->hire(*this);
//                std::cout << "Started new company " << newCompany->product << " / " << newCompany->productivityPerEmployee << " investment = " << newCompany->bankAccount->balance() << std::endl;
            }
        } else {
            auto newEmployer = sim.chooseEmployerByWeight();
            if(newEmployer != sim.companies.end()) {
                if(isEmployed()) employer->endEmployment(*this);
                newEmployer->hire(*this);
            }
        }
    }
}


void Person::spend() {
    if(sim.companies.size() == 0) return;
//    Company *bestCompany = selectProductFromAdvertising();
//    Company *bestCompany = selectBestAvailableProduct();
    Company *bestCompany = selectProductByTrialAndError();
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
    wageExpectation = 100;
    lastWage = 100;
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
        Company &manufacturer = *sim.chooseEmployerByWeight();
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


Company *Person::selectProductByTrialAndError() {
    const double pExploit = 0.9;
    std::vector<Company *> affordableUnknownProducts;
    affordableUnknownProducts.reserve(sim.companies.size()/2);
    Company *bestKnownProduct = nullptr; // best by non-toil wellbeing
    double bestKnownWellbeing = 0.0;
    for(Company &company: sim.companies) {
        if(company.unitPrice < bankAccount->balance() && company.stock >= 1.0) {
            auto knownBrandEntry = knownBrands.find(company.id);
            if(knownBrandEntry != knownBrands.end()) {
                double nonToilWellbeing = knownBrandEntry->second * securityWellbeing(bankAccount->balance() - company.unitPrice);
                if(nonToilWellbeing > bestKnownWellbeing) {
                    bestKnownWellbeing = nonToilWellbeing;
                    bestKnownProduct = &company;
                }
            } else {
                affordableUnknownProducts.push_back(&company);
            }
        }
    }

    if(bestKnownProduct != nullptr && (affordableUnknownProducts.empty() || Random::nextBernoulli(pExploit))) {
        return bestKnownProduct;
    }

    if(affordableUnknownProducts.empty()) return nullptr; // no affordable products

    Company *newProduct = affordableUnknownProducts[Random::nextInt(affordableUnknownProducts.size())];
    knownBrands[newProduct->id] = consumptionWellbeing(newProduct->product);
    return newProduct;
}
