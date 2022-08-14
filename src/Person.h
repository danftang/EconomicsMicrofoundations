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
        CONSUMPTION
    };

    Company *   employer; // employer or null if unemployed. Pointer is managed by employer.
    int         wageExpectation;
    int         lastWage;
    double      consumptionWellbeing;
    std::array<double,3>      mu;       // wellbeing function (of monthly toil and monthly consumption)
    std::array<double,3>      sigma;    // assuming 2D Gaussian with diagonal covaraince matrix

    Person() {
        mu = {1.0 , 0.5};
        sigma = {0.25, 0.25};
        employer = nullptr; // unemployed
        wageExpectation = 1;
        lastWage = 1;
        consumptionWellbeing = 0.0;
    }

    void step();
    void negotiateEmployment();
    void spend();
//    void work(Simulation &);
    void die();
    bool isEmployed() const { return employer != nullptr; }
    double wellbeing(WellbeingDimension d, double x) const;
    double wellbeing() const;
};


#endif //ECONOMICSMICROFOUNDATIONS_PERSON_H
