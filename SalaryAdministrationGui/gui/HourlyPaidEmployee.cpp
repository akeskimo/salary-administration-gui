/*
 * HourlyPaidEmployee.cpp
 *
 *  Created on: 9.8.2016
 *      Author: keskimol
 */

#include "HourlyPaidEmployee.h"


HourlyPaidEmployee::HourlyPaidEmployee(string newName, string newSsn, double newHourlySalary, double newDoneHours)
{
	setName(newName);
	setSocialSecurityNumber(newSsn);
	setHourlySalary(newHourlySalary);
	setDoneHours(newDoneHours);
}

HourlyPaidEmployee::~HourlyPaidEmployee() {}

void HourlyPaidEmployee::setHourlySalary(double newHourlySalary) {
	hourlySalary = newHourlySalary;
}


double HourlyPaidEmployee::getHourlySalary() {
	return hourlySalary;
}


void HourlyPaidEmployee::setDoneHours(double newDoneHours) {
	doneHours = newDoneHours;
}


double HourlyPaidEmployee::getDoneHours() {
	return doneHours;
}


double HourlyPaidEmployee::getSalary() {
  return hourlySalary*doneHours;
}


void HourlyPaidEmployee::printInfo() {
	cout << "Type: " << "HourlyPaidEmployee" << "\n";
	cout << "Name: " << Employee::getName() << "\n";
	cout << "SSN: " << Employee::getSocialSecurityNumber() << "\n";
	cout << "Salary: " << this->getSalary() << "\n";
	cout << ">Done hours: " << this->getDoneHours() << "\n";
	cout << ">Hourly salary: " << this->getHourlySalary() << "\n";
}
