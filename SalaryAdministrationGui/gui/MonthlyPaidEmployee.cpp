/*
 * MonthlyPaidEmployee.cpp
 *
 *  Created on: 9.8.2016
 *      Author: keskimol
 */

#include <string>
#include "MonthlyPaidEmployee.h"


MonthlyPaidEmployee::MonthlyPaidEmployee(string newName, string newSsn, double newMonthlySalary)
{
	Employee::setName(newName);
	Employee::setSocialSecurityNumber(newSsn);
	setMonthlySalary(newMonthlySalary);
};


MonthlyPaidEmployee::~MonthlyPaidEmployee() {}


void MonthlyPaidEmployee::setMonthlySalary(double newMonthlySalary)
{
	monthlySalary = newMonthlySalary;
}


double MonthlyPaidEmployee::getMonthlySalary()
{
	return monthlySalary;
}


double MonthlyPaidEmployee::getSalary()
{
	return monthlySalary;
}

void MonthlyPaidEmployee::printInfo()
{
	cout << "Type: " << "MonthlyPaidEmployee" << "\n";
	cout << "Name: " << Employee::getName() << "\n";
	cout << "SSN: " << Employee::getSocialSecurityNumber() << "\n";
	cout << "Salary: " << this->getSalary() << "\n";
	cout << ">Monthly salary: " << this->getMonthlySalary() << "\n";
}


