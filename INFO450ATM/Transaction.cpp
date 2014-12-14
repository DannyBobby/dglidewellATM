#include "Transaction.h"

// Default Constructor for Transaction objects
Transaction::Transaction()
{
	this->_transactionNumber = 0;
	this->_accountNumber = 0;
	this->_transactionAmount = 0.0;
	this->_date = "------";
	this->_transactionType = ' ';
}

// Typical Constructor for Transaction objects retrieved from Database
Transaction::Transaction(int transactionNum, int acctNum, double transactionAmt, string transactionType, string date)
{
	this->_transactionNumber = transactionNum;
	this->_accountNumber = acctNum;
	this->_transactionAmount = transactionAmt;	
	this->_transactionType = transactionType;
	this->_date = date;
}

// Alternative constructor for use while building a "batch" of Transaction objects.
// We need an alternative, because we don't yet know the transaction number these
// transactions will be given by the DBMS at the time that we create the transaction
// in the ATM logic (performWithdrawal(), performDeposit() functions).
Transaction::Transaction(int acctNum, double transactionAmt, string transactionType)
{
	// We give each of these transactions the transaction number
	// "0" for the reason stated above.
	this->_transactionNumber = 0;
	this->_accountNumber = acctNum;
	this->_transactionAmount = transactionAmt;
	this->_transactionType = transactionType;
	this->_date = "";
}

Transaction::~Transaction()
{
}

#pragma region Get Functions

int Transaction::GetTransactionNumber()
{
	return this->_transactionNumber;
}

int Transaction::GetAccountNumber()
{
	return this->_accountNumber;
}

double Transaction::GetTransactionAmount()
{
	return this->_transactionAmount;
}

string Transaction::GetTransactionType()
{
	return this->_transactionType;
}

string Transaction::GetDate()
{
	return this->_date;
}

#pragma endregion