//
// Created by daniel on 21/07/22.
//

#ifndef ECONOMICSMICROFOUNDATIONS_AGENT_H
#define ECONOMICSMICROFOUNDATIONS_AGENT_H

#include <list>
class Simulation;

class Agent {
public:
    Agent *     employer;
    double      wealth;
    double      wageExpectation;
    double      lastWage;
    std::list<Agent *>  employees;

    Agent(double initialWealth) {
        employer = this; // unemployed
        wealth = initialWealth;
        wageExpectation = initialWealth;
        lastWage = initialWealth;
    }

    enum EmploymentStatus {
        EMPLOYED,
        UNEMPLOYED,
        ENTREPRENEUR
    };

    void step(Simulation &);

    void negotiateEmployment(Simulation &);

    void spend(Simulation &);

    void work(Simulation &);

    void manageBusiness(Simulation &);

    EmploymentStatus status() const {
        if(employer != this) return EMPLOYED;
        if(employees.size() > 0) return ENTREPRENEUR;
        return UNEMPLOYED;
    }

};


#endif //ECONOMICSMICROFOUNDATIONS_AGENT_H
