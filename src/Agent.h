//
// Created by daniel on 21/07/22.
//

#ifndef ECONOMICSMICROFOUNDATIONS_AGENT_H
#define ECONOMICSMICROFOUNDATIONS_AGENT_H

#include <list>

class Agent {
public:
    Agent *     employer;
    double      wealth;
    double      wageExpectation;
    std::list<Agent *>  employees;

    Agent(double initialWealth) {
        employer = this; // unemployed
        wealth = initialWealth;
    }

    enum EmploymentStatus {
        EMPLOYED,
        UNEMPLOYED,
        ENTREPRENEUR
    };

    EmploymentStatus status() {
        if(employer != this) return EMPLOYED;
        if(employees.size() > 0) return ENTREPRENEUR;
        return UNEMPLOYED;
    }

    void negotiateEmployment(Agent &employer);

    double spend();

    double work(double aggregateDemand);

    void manageBusiness();
};


#endif //ECONOMICSMICROFOUNDATIONS_AGENT_H
