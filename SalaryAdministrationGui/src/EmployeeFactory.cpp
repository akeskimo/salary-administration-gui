/*
 * EmployeeFactory.cpp
 *
 *  Created on: 9.8.2016
 *      Author: keskimol
 */

#include "EmployeeFactory.h"
#include "MonthlyPaidEmployee.h"
#include "HourlyPaidEmployee.h"
#include "SalesmanEmployee.h"

using namespace std;

Employee *EmployeeFactory::getEmployee(employee_types type)
{
	switch (type)
	{
	case employee_types::MONTHLY_PAID_EMPLOYEE:
		return new MonthlyPaidEmployee("", "12345678-123", 0.0);
	case employee_types::HOURLY_PAID_EMPLOYEE:
		return new HourlyPaidEmployee("", "12345678-123", 0.0, 0.0);
	case employee_types::SALESMAN_EMPLOYEE:
		return new SalesmanEmployee("", "12345678-123", 0.0, 0.0, false);
	default:
		throw ("Unrecognized employee type: %d", type);
	}
}




////============================================================================
//// Name        : Example.cpp_old
//// Author      :
//// Version     :
//// Copyright   : Your copyright notice
//// Description : Hello World in C++, Ansi-style
////============================================================================
//
//#include<vector>
//#include<iostream>
//using namespace std;
//
//
//
//
//class Stooge
//{
//  public:
//    // Factory Method
//    static Stooge *make_stooge(int choice);
//    virtual void slap_stick() = 0;
//};
//
//int main()
//{
//  vector<Stooge*> roles;
//  int choice;
//  while (true)
//  {
//    cout << "Larry(1) Moe(2) Curly(3) Go(0): ";
//    cin >> choice;
//    if (choice == 0)
//      break;
//    roles.push_back(Stooge::make_stooge(choice));
//  }
//  for (int i = 0; i < roles.size(); i++)
//    roles[i]->slap_stick();
//  for (int i = 0; i < roles.size(); i++)
//    delete roles[i];
//}
//
//class Larry: public Stooge
//{
//  public:
//    void slap_stick()
//    {
//        cout << "Larry: poke eyes\n";
//    }
//};
//class Moe: public Stooge
//{
//  public:
//    void slap_stick()
//    {
//        cout << "Moe: slap head\n";
//    }
//};
//class Curly: public Stooge
//{
//  public:
//    void slap_stick()
//    {
//        cout << "Curly: suffer abuse\n";
//    }
//};
//
//Stooge *Stooge::make_stooge(int choice)
//{
//  if (choice == 1)
//    return new Larry;
//  else if (choice == 2)
//    return new Moe;
//  else
//    return new Curly;
//}
