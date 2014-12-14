#include "Account.h"

#pragma region Constructor & Destructor
Account::Account(int accountNumber, int customerNumber, string accountType, double balance, string accountStatus)
{
	this->_accountNumber = accountNumber;
	this->_customerNumber = customerNumber;
	this->_accountType = accountType;
	this->_balance = balance;
	this->_accountStatus = accountStatus;
}

Account::~Account()
{
}
#pragma endregion

#pragma region Get and Set Functions

int Account::GetAccountNumber()
{
	return this->_accountNumber;
}

int Account::GetCustomerNumber()
{
	return this->_customerNumber;
}

string Account::GetAccountType()
{
	return this->_accountType;
}

double Account::GetAccountBalance()
{
	return this->_balance;
}

void Account::SetAccountBalance(double newAccountBalance)
{
	this->_balance = newAccountBalance;
}

string Account::GetAccountStatus()
{
	return this->_accountStatus;
}

void Account::SetAccountStatus(string status)
{
	this->_accountStatus = status;
}

#pragma endregion