//
// Created by daniel on 01/08/22.
//

#ifndef ECONOMICSMICROFOUNDATIONS_COMPANY_H
#define ECONOMICSMICROFOUNDATIONS_COMPANY_H

#include <list>
#include "Person.h"

class Company: public Agent {
public:
    std::list<Person *>  employees;

    Company(double initialWealth): Agent(initialWealth) { }

    void step(Simulation &sim);
    void endEmployment(Person *employee) { employees.remove(employee); }

    double negotiateWage(double applicantWageExpectation);
};


#endif //ECONOMICSMICROFOUNDATIONS_COMPANY_H
