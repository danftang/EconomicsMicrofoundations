//
// Created by daniel on 21/07/22.
//
//

#ifndef ECONOMICSMICROFOUNDATIONS_PERSON_H
#define ECONOMICSMICROFOUNDATIONS_PERSON_H

#include <array>
#include "Agent.h"

class Simulation;
class Company;

// Each person must consume 1 unit of product per month in order to survive
// however, different products lead to different wellbeing
class Person: public Agent {
public:
    enum WellbeingDimension {
        TOIL,
        CONSUMPTION,
        SECURITY
    };

    constexpr static int     initialAgentWealth = 1000;
    Company *   employer; // employer or null if unemployed. Pointer is managed by employer.
    int         wageExpectation;
    int         lastWage;
    double      currentConsumptionWellbeing;
    std::array<double,3>      mu;       // wellbeing function (of monthly toil and monthly consumption)
    std::array<double,3>      sigma;    // assuming 2D Gaussian with diagonal covaraince matrix

    Person();

    void step();
    void negotiateEmployment();
    void spend();
//    void work(Simulation &);
    void die();
    bool isEmployed() const { return employer != nullptr; }
    double consumptionWellbeing(double product) const { double x = (product-mu[CONSUMPTION])/sigma[CONSUMPTION]; return exp(-x*x); }
    double toilWellbeing() const;
    double securityWellbeing() const;
    double securityWellbeing(double balance) const;
    double wellbeing() const;

protected:
    Company *selectProductFromAdvertising();
    Company *selectBestAvailableProduct();
};


#endif //ECONOMICSMICROFOUNDATIONS_PERSON_H
