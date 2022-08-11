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
    Company *   employer; // employer or null if unemployed
    double      shares;
    double      wageExpectation;
    double      lastWage;

    Person(double initialWealth): Agent(initialWealth) {
        employer = nullptr; // unemployed
        wageExpectation = initialWealth;
        lastWage = initialWealth;
    }

    void step(Simulation &);

    void negotiateEmployment(Simulation &);

    void spend(Simulation &);

    void work(Simulation &);

    void invest(Simulation &);

    bool isEmployed() const { return employer != nullptr; }

    void startWorkingFor(Company *newEmployer);

//    EmploymentStatus status() const {
//        if(employer != this) return EMPLOYED;
//        if(employees.size() > 0) return ENTREPRENEUR;
//        return UNEMPLOYED;
//    }

};


#endif //ECONOMICSMICROFOUNDATIONS_PERSON_H
