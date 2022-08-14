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


    std::list<Person *>     employees;
    Bank::AccountID         loanAccount;
    const double            product;    // sugar/spice mix
    const double            toilPerUnitproduct;
    double                  stock;      // amount of product available
    double                  productivityPerEmployee; // how hard do employees work?
    int                     unitPrice;

    Company(double product);
    ~Company();

    void step();
    void endEmployment(Person &employee);
    void hire(Person *employee);
    int negotiateWage(Person &);
    void sanityCheck();
};


#endif //ECONOMICSMICROFOUNDATIONS_COMPANY_H
