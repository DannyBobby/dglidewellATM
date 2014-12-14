#ifndef ATM_H
#define ATM_H

#pragma once

#include <string>
#include <stack>
#include "Customer.h"
#include "UI.h"
#include "Account.h"
#include "Database.h"
#include "Transaction.h"

using namespace std;


class ATM
{
	bool atmShutdown = false;
	string email = "";
	int pin = NULL;

public:
	ATM();
	~ATM();

	bool GetAtmShutdown();

	//ATM Functions
	bool Login();
	void CreateNewCustomer();
	void MainMenu();
	void PerformBalanceInquiry();
	void PerformWithdrawal();
	void PerformDeposit();
	void PerformTransfer();

	// These functions should be consolidated
	// to reduce redundancy.
	void ShowTransactionHistory();	
	void ShowTransferHistory();
		
	void ManageAccount();    //<-- Change status of account to INACTIVE
	void LogoutCustomer();   //<-- Clean-up procedure to prepare for the next user
	void ShutdownATM();      //<-- Procedure to shutdown the ATM; removing objects 
							 //    which were instantiated on the heap.
};

#endif