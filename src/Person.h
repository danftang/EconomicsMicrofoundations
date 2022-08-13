//
// Created by daniel on 21/07/22.
//
//

#ifndef ECONOMICSMICROFOUNDATIONS_PERSON_H
#define ECONOMICSMICROFOUNDATIONS_PERSON_H

#include "Agent.h"

class Simulation;
class Company;

class Person: public Agent {
public:
    Company *   employer; // employer or null if unemployed. Pointer is managed by employer.
    int         wageExpectation;
    int         lastWage;

    Person() {
        employer = nullptr; // unemployed
//        wageExpectation = initialWageExpectation;
//        lastWage = initialWageExpectation;
    }

    void step(Simulation &);
    void negotiateEmployment(Simulation &);
    void spend(Simulation &);
    void work(Simulation &);
    void die(Simulation &);
    bool isEmployed() const { return employer != nullptr; }
};


#endif //ECONOMICSMICROFOUNDATIONS_PERSON_H
