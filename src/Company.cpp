//
// Created by daniel on 01/08/22.
//

#include <cassert>
#include "Company.h"
#include "Simulation.h"
#include "Person.h"
#include "mystd/Random.h"

void Company::step(Simulation &sim) {
    auto employeeIt = employees.begin();
    if(loanAccount->balance() != 0) {
        int loanRepayment = std::min(bankAccount->balance(), -loanAccount->balance()/5 + 1);
        sim.bank.transfer(bankAccount, loanAccount, loanRepayment);
    }
    while(employeeIt != employees.end()) {
        assert((*employeeIt)->employer == this);
        if (bankAccount->balance() >= (*employeeIt)->wageExpectation) {
            sim.bank.transfer(bankAccount, (*employeeIt)->bankAccount, (*employeeIt)->wageExpectation);
            (*employeeIt)->lastWage = (*employeeIt)->wageExpectation;
            ++employeeIt;
        } else { // fire employee
            (*employeeIt)->wageExpectation = Random::nextInt(1,(*employeeIt)->lastWage+1); // DIFFERENT FROM PAPER, but needed to reproduce results
            (*employeeIt)->employer = nullptr;
            employeeIt = employees.erase(employeeIt);
        }
    }
}

int Company::negotiateWage(int applicantWageExpectation) {
    int negotiatedWage = Random::nextInt(applicantWageExpectation, 2 * applicantWageExpectation + 1);
    if (negotiatedWage > bankAccount->balance()) negotiatedWage = bankAccount->balance();
    return negotiatedWage;
}

Company::Company() : Agent(), loanAccount(sim.bank.openLoanAccount()) { }

Company::~Company() {
    sim.bank.transfer(bankAccount, loanAccount, std::min(bankAccount->balance(), -loanAccount->balance())); // pay off any debt
    sim.bank.closeLoanAccount(loanAccount);
}

void Company::sanityCheck() {
    for(const Person *employee: employees) assert(employee->employer == this);
}

void Company::endEmployment(Person *employee) {
    assert(employee->employer == this);
    employee->employer = nullptr;
    employees.remove(employee);
}

void Company::hire(Person *employee) {
    assert(employee->employer == nullptr);
    employee->employer = this;
    employees.push_back(employee);
}
