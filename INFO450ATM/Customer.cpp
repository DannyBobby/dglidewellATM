#include "Customer.h"


Customer::Customer()
{
}

Customer::Customer(int custNum, string lastName, string firstName, string emailAddress, int pin)
{
	this->_customerNumber = custNum;	
	this->_custLastName = lastName;
	this->_custFirstName = firstName;	
	this->_emailAddress = emailAddress;
	this->_PIN = pin;
}

Customer::~Customer()
{
}

#pragma region Get Functions

int Customer::GetCustomerNumber()
{
	return this->_customerNumber;
}

string Customer::GetEmailAddress()
{
	return this->_emailAddress;
}

string Customer::GetFirstName()
{
	return this->_custFirstName;
}

string Customer::GetLastName()
{
	return this->_custLastName;
}

int Customer::GetPIN()
{
	return this->_PIN;
}

#pragma endregion

#pragma region Set Functions

void Customer::SetFirstName(string fname)
{
	this->_custFirstName = fname;
}

void Customer::SetLastName(string lname)
{
	this->_custLastName = lname;
}

void Customer::SetEmailAddress(string email)
{
	this->_emailAddress = email;
}

void Customer::SetPIN(int pin)
{
	this->_PIN = pin;
}

#pragma endregion