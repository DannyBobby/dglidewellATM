#include "Transfer.h"

// Typical constructor for Transfer object
Transfer::Transfer(int transactionNum, int sourceAcctNum, int destinationAcctNum, double transactionAmt, string date)
{
	this->_transactionNumber = transactionNum;
	this->_accountNumber = sourceAcctNum;
	this->_destinationAccountNumber = destinationAcctNum;
	this->_transactionAmount = transactionAmt;
	this->_transactionType = "T";
	this->_date = date;	
}

// We need an alternative constructor, because we don't yet know the transaction number these
// transactions will be given by the DBMS at the time that we create the transaction
// in the ATM logic (performWithdrawal(), performDeposit() functions).
Transfer::Transfer(int sourceAcctNum, int destinationAcctNum, double transactionAmt)
{
	this->_transactionNumber = 0;
	this->_accountNumber = sourceAcctNum;
	this->_destinationAccountNumber = destinationAcctNum;
	this->_transactionAmount = transactionAmt;
	this->_transactionType = "T";
	this->_date = "";
}


Transfer::~Transfer()
{
}

// Get Function
int Transfer::GetDestinationAccountNumber()
{
	return this->_destinationAccountNumber;
}