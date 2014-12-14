#ifndef CUSTOMER_H
#define CUSTOMER_H

#pragma once
#include "Person.h"
#include <string>

using namespace std;

// Customer inherits all those basic attributes
// of the Person class plus some customer-specific
// attributes and behaviors.
class Customer :
	public Person
{
private:
	int _customerNumber;
	string _emailAddress;
	string _custFirstName;
	string _custLastName;
	int _PIN;

public:	
	Customer();
	Customer(int custNum, string lastName, string firstName, string emailAddress, int pin);
	~Customer();

	// Get Functions
	int GetCustomerNumber();
	string GetEmailAddress();
	string GetFirstName();
	string GetLastName();
	int GetPIN();

	// Set Functions
	void SetFirstName(string fname);
	void SetLastName(string lname);
	void SetEmailAddress(string email);
	void SetPIN(int pin);
};

#endif