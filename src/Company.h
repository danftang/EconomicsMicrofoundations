//
// Created by daniel on 01/08/22.
//

#ifndef ECONOMICSMICROFOUNDATIONS_COMPANY_H
#define ECONOMICSMICROFOUNDATIONS_COMPANY_H

#include <list>
#include "Bank.h"
#include "Person.h"

class Company: public Agent {
public:
    constexpr static const double            profitRatio = 1.1;

    std::list<Person *>     employees;
    Bank::AccountID         loanAccount;

    Company();
    ~Company();

    void step(Simulation &sim);
    void endEmployment(Person *employee);
    void hire(Person *employee);
    int negotiateWage(int applicantWageExpectation);
    void sanityCheck();
};


#endif //ECONOMICSMICROFOUNDATIONS_COMPANY_H
