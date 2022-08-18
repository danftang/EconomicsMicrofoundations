//
// Created by daniel on 01/08/22.
//

#include <cassert>
#include "Company.h"
#include "Simulation.h"
#include "Person.h"
#include "mystd/Random.h"

Company::Company(double product, double productivity) :
        Agent(),
        loanAccount(sim.bank.openLoanAccount()),
        product(product),
        stock(0.0),
        productivityPerEmployee(productivity),
        toilPerUnitproduct(SPICE_TIOL*product + SUGAR_TOIL*(1.0-product)),
        age(0) { }

Company::~Company() {
    sim.bank.transfer(bankAccount, loanAccount, std::min(bankAccount->balance(), -loanAccount->balance())); // pay off any debt
    sim.bank.closeLoanAccount(loanAccount);
}

void Company::step() {
    double costOfProduction = bankAccount->balance(); // ... we'll subtract balance after costs
    if(loanAccount->balance() != 0) {
        int loanRepayment = std::min(bankAccount->balance(), -loanAccount->balance()/8 + 1);
        sim.bank.transfer(bankAccount, loanAccount, loanRepayment);
    }
    // payroll and firing
    std::shuffle(employees.begin(), employees.end(), Random::gen);
    auto employeeIt = employees.begin();
    while(employeeIt != employees.end()) {
        if (bankAccount->balance() >= (*employeeIt)->wageExpectation) { // pay employee
            sim.bank.transfer(bankAccount, (*employeeIt)->bankAccount, (*employeeIt)->wageExpectation);
            (*employeeIt)->lastWage = (*employeeIt)->wageExpectation;
            ++employeeIt;
        } else { // fire employee
            (*employeeIt)->wageExpectation = Random::nextInt(1,(*employeeIt)->lastWage+1);
            endEmployment(employeeIt);
        }
    }
    // production and price setting
    costOfProduction -= bankAccount->balance();
    double production = employees.size() * productivityPerEmployee / toilPerUnitproduct;
    stock += production;
//    unitPrice = 1 + (int)(costOfProduction * (1.0 + profitMargin) / production);
    unitPrice = 1 + (int)(costOfProduction * (1.0 + profitMargin) / stock);
    ++age;
}

int Company::negotiateWage(Person &applicant) {
    if(applicant.employer == this) return applicant.wageExpectation;
    int negotiatedWage = Random::nextInt(applicant.wageExpectation, 2 * applicant.wageExpectation + 1);
    if (negotiatedWage > bankAccount->balance()) negotiatedWage = bankAccount->balance();
    return negotiatedWage;
}


void Company::sanityCheck() {
    for(const Person *employee: employees) assert(employee->employer == this);
}

void Company::endEmployment(Person &employee) {
    auto employeeIt = std::find(employees.begin(), employees.end(), &employee);
    if(employeeIt != employees.end()) endEmployment(employeeIt);
}


void Company::endEmployment(std::vector<Person *>::iterator employeeIt) {
    assert((*employeeIt)->employer == this);
    (*employeeIt)->employer = nullptr;
    *employeeIt = employees[employees.size()-1];
    employees.pop_back();
}


void Company::hire(Person *employee) {
    assert(employee->employer == nullptr);
    employee->employer = this;
    employees.push_back(employee);
}
