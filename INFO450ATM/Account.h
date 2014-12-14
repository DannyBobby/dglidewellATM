#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>

using namespace std;

#pragma once

class Account
{
private:
	int _accountNumber;
	int _customerNumber;
	// variable is necessary in case the ATM expands 
	// its functionality to accomodate more than just 
	// checking accounts.
	string _accountType; 
	double _balance;
	string _accountStatus;

public:	
	Account(int accountNumber, int customerNumber, string accountType, double balance, string accountStatus);
	~Account();

	// Get and Set functions for the 
	// private attributes above
	int GetAccountNumber();
	int GetCustomerNumber();
	string GetAccountType();
	double GetAccountBalance();
	string GetAccountStatus();
	void SetAccountStatus(string status);
	void SetAccountBalance(double newAccountBalance);
};

#endif