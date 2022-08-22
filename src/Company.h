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
    constexpr static const double   profitMargin = 0.1;
    constexpr static const double   SUGAR_TOIL = 0.5; // person-months of standard-productivity toil needed to grow 1 unit of sugar
    constexpr static const double   SPICE_TIOL = 1.5; // person-months of standard-productivity toil needed to grow 1 unit of spice


    std::vector<Person *>   employees;
    Bank::AccountID         loanAccount;
    const double            product;    // sugar/spice mix
    const double            toilPerUnitproduct;
    const double            productivityPerEmployee; // how hard do employees work?
    double                  stock;      // amount of product available
    int                     unitPrice;
    int                     age;    // of company in months (steps)

    Company(double product, double productivity);
    ~Company();

    void step();
    void hire(Person &employee);
    int negotiateWage(Person &);
    void endEmployment(Person &employee);
    void endEmployment(std::vector<Person *>::iterator employeeIt);
    void sanityCheck();
//    double weight() { return bankAccount->balance(); }
//    double weight() { return employees.size(); }
    double weight() { return 1.0; }
};


#endif //ECONOMICSMICROFOUNDATIONS_COMPANY_H
