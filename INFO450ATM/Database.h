// This class was based on the "DataContext" class given by the professor

#pragma once
#include "KompexSQLitePrerequisites.h"
#include "KompexSQLiteDatabase.h"
#include "KompexSQLiteStatement.h"
#include "KompexSQLiteException.h"
#include "KompexSQLiteStreamRedirection.h"
#include "KompexSQLiteBlob.h"
#include "sqlite3.h"
#include "sqlite3ext.h"

#include <iostream>
#include <exception>
#include <vector>
#include <stack>
#include "Account.h"
#include "Customer.h"
#include "Transaction.h"
#include "Transfer.h"
#include "Page.h"

using namespace Kompex;
using namespace std;

class Database
{
private:

	// Private functions for the Database object to use to create
	// Pointers to SQLiteDatabase and SQLiteStatement objects.
	SQLiteDatabase *connect() {
		return new SQLiteDatabase("atmdata.db", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 0);
	}

	SQLiteStatement *createStatement(SQLiteDatabase *pDatabase) {
		return new SQLiteStatement(pDatabase);
	}

public:

#pragma region Constructor & Destructor
	// Initialize our database for usage,
	// ensuring that all tables are provisioned
	Database() 
	{
		// create and open database
		SQLiteDatabase *pDatabase = this->connect();
		// create statement instance for sql queries/statements
		SQLiteStatement *pStmt = this->createStatement(pDatabase);
		
		pStmt->SqlStatement("CREATE TABLE IF NOT EXISTS Customer (customerNumber INTEGER NOT NULL PRIMARY KEY, lastName VARCHAR(80), firstName VARCHAR(80), emailAddress VARCHAR(80) NOT NULL, PIN INTEGER NOT NULL)");		
		pStmt->SqlStatement("CREATE TABLE IF NOT EXISTS Account (accountNumber INTEGER NOT NULL PRIMARY KEY, customerNumber INTEGER NOT NULL, accountType CHAR NOT NULL, balance FLOAT NOT NULL, status VARCHAR(8) DEFAULT 'ACTIVE' NOT NULL)");				
		pStmt->SqlStatement("CREATE TABLE IF NOT EXISTS AccountTransaction (transactionNumber INTEGER NOT NULL PRIMARY KEY, accountNumber INTEGER NOT NULL, transactionAmount FLOAT NOT NULL, transactionType VARCHAR(1) NOT NULL, date TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL)");
		pStmt->SqlStatement("CREATE TABLE IF NOT EXISTS AccountTransfer (transferNumber INTEGER NOT NULL PRIMARY KEY, sourceAccountNumber INTEGER NOT NULL, destinationAccount INTEGER NOT NULL, transactionAmount FLOAT NOT NULL, date TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL)");

		// De-allocate memory used to store pointers
		delete pDatabase;
		delete pStmt;
	}

	// Destructor
	~Database()
	{
	}
#pragma endregion

#pragma region Functions related to Customer objects
	// Create Customer Function: 
	// This is the function used to create a customer entry in the database.
	Customer *createCustomer(string lastName, string firstName, string emailAddress, int pin) 
	{
		// First create a pointer to a SQLiteDatabase using 
		// the connect() function defined above and then
		// create a pointer to an SQLiteStatement object.		
		SQLiteDatabase *pDatabase = this->connect();
		SQLiteStatement *pStmt = this->createStatement(pDatabase);  // Notice how the SQLiteStatement 
																	// pointer (pStmt) is "tied" to the 
		                                                            // SQLiteDatabase object that pDatabase 
																	// points to.

		// Use the SQLiteStatement pointer (pStmt) created 
		// above to send a SQL statement to the database.
		pStmt->Sql("INSERT INTO Customer (lastName, firstName, emailAddress, PIN) VALUES(?, ?, ?, ?);");
		pStmt->BindString(1, lastName);               // First question mark in the VALUES() clause above
		pStmt->BindString(2, firstName);              // Second question mark in the VALUES() clause above
		pStmt->BindString(3, emailAddress);           // Third question mark in the VALUES() clause above
		pStmt->BindInt(4, pin);                       // Fourth question mark in the VALUES() clause above

		// executes the INSERT statement and cleans-up automatically
		pStmt->ExecuteAndFree();

		// get the customer ID for the customer entry which was just made (the last row id).
		pStmt->Sql("SELECT last_insert_rowid();");
		pStmt->Execute();
		int customerNumber = pStmt->GetColumnInt(0); // get the int value at the zeroth column 
		pStmt->FreeQuery();

		/*************************************************************************************** 
		 * Notice how each pStmt is first "loaded" with an SQL statement via the Sql() function
		 * then executed with either the Execute() or ExecuteAndFree() functions.
		 ***************************************************************************************/

		// De-allocate memory used to store pointers
		delete pDatabase;
		delete pStmt;

		// return the customer to the calling function
		return new Customer(customerNumber, lastName, firstName, emailAddress, pin);
	}	

	// Retrieve Customer Function:
	// Function which searches the Customer table and returns a customer object based on the 
	// emailAddress which is passed to it.   NEEDS INPUT VALIDATION FOR "email" !!!
	Customer *getCustomer(string email) 
	{
		// Here are our variables which store the values which will be returned by 
		// the database search (assuming the search was successful)
		int retrievedCustomerID = 0;
		string retrievedFirstName = "";
		string retrievedLastName = "";
		int retrievedPIN = 0;
		string retrievedEmailAddress = "";

		// First create a pointer to a SQLiteDatabase using 
		// the connect() function defined above and then
		// create a pointer to an SQLiteStatement object.		
		SQLiteDatabase *pDatabase = this->connect();
		SQLiteStatement *pStmt = this->createStatement(pDatabase);

		// Use the email address passed to this method to query the database.
		pStmt->Sql("SELECT * FROM Customer WHERE emailAddress = ?;");
		pStmt->BindString(1, email);      // First question mark in the SQL statement above

		// Process the results of the query above - assigning the values of each
		// column to the variables declared above.
		while (pStmt->FetchRow())
		{
			retrievedCustomerID = pStmt->GetColumnInt("customerNumber");			
			retrievedLastName = pStmt->GetColumnString("lastName");
			retrievedFirstName = pStmt->GetColumnString("firstName");
			retrievedEmailAddress = pStmt->GetColumnString("emailAddress");
			retrievedPIN = pStmt->GetColumnInt("PIN");			
		}

		// "Clean up"
		pStmt->FreeQuery();

		// De-allocate memory used to store pointers
		delete pDatabase;
		delete pStmt;

		// Use the variables, which have been assigned values via the query above, 
		// to create a Customer object to return.
		return new Customer(retrievedCustomerID, retrievedLastName, retrievedFirstName, retrievedEmailAddress, retrievedPIN);
	}

	// Delete Customer Function:
	// Searches the Customer table and deletes the row(s) corresponding to the 
	// email address which is passed to it.  Returns false if no changes were
	// made and true if at least one was made.
	bool deleteCustomer(string email)
	{
		// First create a pointer to a SQLiteDatabase using 
		// the connect() function defined above and then
		// create a pointer to an SQLiteStatement object.		
		SQLiteDatabase *pDatabase = this->connect();
		SQLiteStatement *pStmt = this->createStatement(pDatabase);

		// SQL Statement to delete the record
		pStmt->Sql("DELETE FROM Customer WHERE emailAddress = ?;");
		pStmt->BindString(1, email);
		pStmt->ExecuteAndFree();

		 /*Check to see if any changes have been made to the database
		 due to the SQL statement executed above.  Return the results
		 as a boolean value.*/
		int recordsDeleted = 0;
		recordsDeleted = pDatabase->GetTotalDatabaseChanges();
		if (recordsDeleted)
		{
			// De-allocate memory used to store pointers
			delete pDatabase;
			delete pStmt;
			return true;
		}			
		else
		{
			// De-allocate memory used to store pointers
			delete pDatabase;
			delete pStmt;
			return false;
		}
	}
#pragma endregion 

#pragma region Functions related to Account objects
	// Create Account Function
	Account *createAccount(int customerNumber, string accountType)
	{
		double initialBalance = 0.0;
		
		// First create a pointer to a SQLiteDatabase using 
		// the connect() function defined above and then
		// create a pointer to an SQLiteStatement object.		
		SQLiteDatabase *pDatabase = this->connect();
		SQLiteStatement *pStmt = this->createStatement(pDatabase);  // Notice how the SQLiteStatement 
																	// pointer (pStmt) is "tied" to the 
																	// SQLiteDatabase object that pDatabase 
																	// points to.

		// Use the SQLiteStatement pointer (pStmt) created 
		// above to send a SQL statement to the database.
		pStmt->Sql("INSERT INTO Account (customerNumber, accountType, balance) VALUES(?, ?, ?);");
		pStmt->BindInt(1, customerNumber);         // First question mark in the VALUES() clause above
		pStmt->BindString(2, accountType);         // Second question mark in the VALUES() clause above
		pStmt->BindDouble(3, initialBalance);      // Third question mark in the VALUES() clause above		

		// executes the INSERT statement and cleans-up automatically
		pStmt->ExecuteAndFree();

		// get the customer ID for the customer entry which was just made (the last row id).
		pStmt->Sql("SELECT last_insert_rowid();");
		pStmt->Execute();
		int accountNumber = pStmt->GetColumnInt(0); // get the int value at the zeroth column 
		pStmt->FreeQuery();

		/***************************************************************************************
		* Notice how each pStmt is first "loaded" with an SQL statement via the Sql() function
		* then executed with either the Execute() or ExecuteAndFree() functions.
		***************************************************************************************/

		// De-allocate memory used to store pointers
		delete pDatabase;
		delete pStmt;

		// return the account to the calling function
		return new Account(accountNumber, customerNumber, accountType, initialBalance, "ACTIVE");
	}
	
	// Function used to retrieve account info using the customer number
	Account *getAccount(int customerNumber, string accountType)  // <-- Default accountType is Checking or "C"
	{
		// Here are our variables which store the values which will be returned by 
		// the database search (assuming the search was successful)
		int retrievedAccountNumber = 0;
		int retrievedCustomerNumber = 0;
		string retrievedAccountType = "";
		double retrievedBalance = 0.0;
		string retrievedStatus = "";

		// First create a pointer to a SQLiteDatabase using 
		// the connect() function defined above and then
		// create a pointer to an SQLiteStatement object.		
		SQLiteDatabase *pDatabase = this->connect();
		SQLiteStatement *pStmt = this->createStatement(pDatabase);
		
		// Use the customerNumber and accountType passed to this method to query the database.
		pStmt->Sql("SELECT * FROM Account WHERE customerNumber = ? AND accountType = ?;");
		pStmt->BindInt(1, customerNumber);
		pStmt->BindString(2, accountType);	

		// Process the results of the query above - assigning the values of each
		// column to the variables declared above.
		while (pStmt->FetchRow())
		{
			retrievedAccountNumber = pStmt->GetColumnInt("accountNumber");
			retrievedCustomerNumber = pStmt->GetColumnInt("customerNumber");
			retrievedAccountType = pStmt->GetColumnString("accountType");
			retrievedBalance = pStmt->GetColumnDouble("balance");
			retrievedStatus = pStmt->GetColumnString("status");
		}

		// "Clean up"
		pStmt->FreeQuery();

		// De-allocate memory used to store pointers
		delete pDatabase;
		delete pStmt;

		// Use the variables, which have been assigned values via the query above, 
		// to create a Customer object to return.
		return new Account(retrievedAccountNumber, retrievedCustomerNumber, retrievedAccountType, retrievedBalance, retrievedStatus);
	}
	
	// Delete Account Function:
	// I should consider making a more abstract method called 
	// "deleteRow" whereby one can specify the table name and
	// the primary key of the row to be deleted.
	bool deleteAccount(int accountNumber)
	{
		// First create a pointer to a SQLiteDatabase using 
		// the connect() function defined above and then
		// create a pointer to an SQLiteStatement object.		
		SQLiteDatabase *pDatabase = this->connect();
		SQLiteStatement *pStmt = this->createStatement(pDatabase);

		// SQL Statement to delete the record
		pStmt->Sql("DELETE FROM Account WHERE accountNumber = ?;");
		pStmt->BindInt(1, accountNumber);
		pStmt->ExecuteAndFree();

		// Check to see if any changes have been made to the database
		// due to the SQL statement executed above.  Return the results
		// as a boolean value.
		int recordsDeleted = 0;
		recordsDeleted = pDatabase->GetTotalDatabaseChanges();
		if (recordsDeleted)
		{
			// De-allocate memory used to store pointers
			delete pDatabase;
			delete pStmt;
			return true;
		}
		else
		{
			// De-allocate memory used to store pointers
			delete pDatabase;
			delete pStmt;
			return false;
		}
	}

	// updateBalance() takes an accountNumber and a double and uses each
	// to change the value stored in the "balance" column for a particular
	// row in the Account table.
	void updateBalance(int accountNumber, double newBalance)
	{
		// First create a pointer to a SQLiteDatabase using 
		// the connect() function defined above and then
		// create a pointer to an SQLiteStatement object.		
		SQLiteDatabase *pDatabase = this->connect();
		SQLiteStatement *pStmt = this->createStatement(pDatabase);
	
		pStmt->Sql("UPDATE Account SET balance = ? WHERE accountNumber = ?;");
		pStmt->BindDouble(1, newBalance);     // First question mark in the VALUES() clause above
		pStmt->BindInt(2, accountNumber);     // Second question mark in the VALUES() clause above
		pStmt->ExecuteAndFree();

		// De-allocate memory used to store pointers
		delete pDatabase;
		delete pStmt;
	}

	// updateStatus() takes an accountNumber and a string and uses each
	// to change the value stored in the "status" column for a particular
	// row in the Account table.
	void updateStatus(int accountNumber, string status)
	{
		// First create a pointer to a SQLiteDatabase using 
		// the connect() function defined above and then
		// create a pointer to an SQLiteStatement object.		
		SQLiteDatabase *pDatabase = this->connect();
		SQLiteStatement *pStmt = this->createStatement(pDatabase);

		pStmt->Sql("UPDATE Account SET status = ? WHERE accountNumber = ?;");
		pStmt->BindString(1, status);            // First question mark in the VALUES() clause above
		pStmt->BindInt(2, accountNumber);        // Second question mark in the VALUES() clause above
		pStmt->ExecuteAndFree();

		// De-allocate memory used to store pointers
		delete pDatabase;
		delete pStmt;
	}
#pragma endregion

#pragma region Functions related to Transaction objects

	// Used to write new rows to the AccountTransaction table
	void createTransaction(int accountNumber, double transactionAmt, string transactionType)
	{
		// First create a pointer to a SQLiteDatabase using 
		// the connect() function defined above and then
		// create a pointer to an SQLiteStatement object.		
		SQLiteDatabase *pDatabase = this->connect();
		SQLiteStatement *pStmt = this->createStatement(pDatabase);  // Notice how the SQLiteStatement 
																    // pointer (pStmt) is "tied" to the 
		                                                            // SQLiteDatabase object that pDatabase 
		                                                            // points to.		
		// Use the SQLiteStatement pointer (pStmt) created 
		// above to send a SQL statement to the database.
		pStmt->Sql("INSERT INTO AccountTransaction (accountNumber, transactionAmount, transactionType) VALUES(?, ?, ?);");
		pStmt->BindInt(1, accountNumber);        // First question mark in the VALUES() clause above
		pStmt->BindDouble(2, transactionAmt);    // Second question mark in the VALUES() clause above
		pStmt->BindString(3, transactionType);   // Third question mark in the VALUES() clause above		

		// executes the INSERT statement and cleans-up automatically
		pStmt->ExecuteAndFree();

		// De-allocate memory used to store pointers
		delete pDatabase;
		delete pStmt;
	}

	// Used to return a particular transaction from the AccountTransaction table if one knows
	// the transaction number.
	Transaction *getTransaction(int transactionNumber)
	{
		// Here are our variables which store the values which will be returned by 
		// the database search (assuming the search was successful)
		int retrievedTransactionNumber = 0;
		int retrievedAccountNumber = 0;
		double retrievedTransactionAmt = 0.0;		
		string retrievedTransactionType = "";	
		string retrievedDate = "";

		// First create a pointer to a SQLiteDatabase using 
		// the connect() function defined above and then
		// create a pointer to an SQLiteStatement object.		
		SQLiteDatabase *pDatabase = this->connect();
		SQLiteStatement *pStmt = this->createStatement(pDatabase);
		
		// Use the customerNumber and accountType passed to this method to query the database.
		pStmt->Sql("SELECT * FROM AccountTransaction WHERE transactionNumber = ?;");
		pStmt->BindInt(1, transactionNumber);       

		// Process the results of the query above - assigning the values of each
		// column to the variables declared above.
		while (pStmt->FetchRow())
		{
			retrievedTransactionNumber = pStmt->GetColumnInt("transactionNumber");
			retrievedAccountNumber = pStmt->GetColumnInt("accountNumber");
			retrievedTransactionAmt = pStmt->GetColumnDouble("transactionAmount");			
			retrievedTransactionType = pStmt->GetColumnString("transactionType");
			retrievedDate = pStmt->GetColumnString("date");
		}

		// "Clean up"
		pStmt->FreeQuery();		

		// De-allocate memory used to store pointers
		delete pDatabase;
		delete pStmt;

		// Use the variables, which have been assigned values via the query above, 
		// to create a Customer object to return.
		return new Transaction(retrievedTransactionNumber, retrievedAccountNumber, retrievedTransactionAmt, retrievedTransactionType, retrievedDate);
	}

	// Potential function to be used to edit rows in the AccountTransaction table
	/*bool editTransaction()
	{		
		return false;
	}*/

	// Delete a row from the AccountTransaction table
	bool deleteTransaction(int transactionNumber)
	{
		// First create a pointer to a SQLiteDatabase using 
		// the connect() function defined above and then
		// create a pointer to an SQLiteStatement object.		
		SQLiteDatabase *pDatabase = this->connect();
		SQLiteStatement *pStmt = this->createStatement(pDatabase);

		// SQL Statement to delete the record
		pStmt->Sql("DELETE FROM AccountTransaction WHERE transactionNumber = ?;");
		pStmt->BindInt(1, transactionNumber);
		pStmt->ExecuteAndFree();

		// Check to see if any changes have been made to the database
		// due to the SQL statement executed above.  Return the results
		// as a boolean value.
		int recordsDeleted = 0;
		recordsDeleted = pDatabase->GetTotalDatabaseChanges();
		if (recordsDeleted)
		{
			// De-allocate memory used to store pointers
			delete pDatabase;
			delete pStmt;
			return true;
		}
		else
		{
			// De-allocate memory used to store pointers
			delete pDatabase;
			delete pStmt;
			return false;
		}
	}

#pragma endregion

#pragma region Functions related to Transfer objects
	
	// Create a row in the AccountTransfer table
	void createTransfer(int sourceAccountNumber, int destinationAccount, double transactionAmt)
	{
		// First create a pointer to a SQLiteDatabase using 
		// the connect() function defined above and then
		// create a pointer to an SQLiteStatement object.		
		SQLiteDatabase *pDatabase = this->connect();
		SQLiteStatement *pStmt = this->createStatement(pDatabase);  // Notice how the SQLiteStatement 
		// pointer (pStmt) is "tied" to the 
		// SQLiteDatabase object that pDatabase 
		// points to.		
		// Use the SQLiteStatement pointer (pStmt) created 
		// above to send a SQL statement to the database.
		pStmt->Sql("INSERT INTO AccountTransfer (sourceAccountNumber, destinationAccount, transactionAmount) VALUES(?, ?, ?);");
		pStmt->BindInt(1, sourceAccountNumber);       // First question mark in the VALUES() clause above
		pStmt->BindInt(2, destinationAccount);        // Second question mark in the VALUES() clause above
		pStmt->BindDouble(3, transactionAmt);         // Third question mark in the VALUES() clause above		

		// executes the INSERT statement and cleans-up automatically
		pStmt->ExecuteAndFree();

		// De-allocate memory used to store pointers
		delete pDatabase;
		delete pStmt;
	}

	Transfer *getTransfer(int transferNumber)
	{
		// Here are our variables which store the values which will be returned by 
		// the database search (assuming the search was successful)
		int retrievedTransferNumber = 0;
		int retrievedSourceAccountNumber = 0;
		int retrievedDestinationAccountNumber = 0;
		double retrievedTransferAmt = 0.0;
		string retrievedDate = "";

		// First create a pointer to a SQLiteDatabase using 
		// the connect() function defined above and then
		// create a pointer to an SQLiteStatement object.		
		SQLiteDatabase *pDatabase = this->connect();
		SQLiteStatement *pStmt = this->createStatement(pDatabase);

		// Use the customerNumber and accountType passed to this method to query the database.
		pStmt->Sql("SELECT * FROM AccountTransfer WHERE transferNumber = ?;");
		pStmt->BindInt(1, transferNumber);

		// Process the results of the query above - assigning the values of each
		// column to the variables declared above.
		while (pStmt->FetchRow())
		{
			retrievedTransferNumber = pStmt->GetColumnInt("transferNumber");
			retrievedSourceAccountNumber = pStmt->GetColumnInt("sourceAccountNumber");
			retrievedDestinationAccountNumber = pStmt->GetColumnInt("destinationAccount");
			retrievedTransferAmt = pStmt->GetColumnDouble("transactionAmount");
			retrievedDate = pStmt->GetColumnString("date");
		}

		// "Clean up"
		pStmt->FreeQuery();

		// De-allocate memory used to store pointers
		delete pDatabase;
		delete pStmt;

		// Use the variables, which have been assigned values via the query above, 
		// to create a Customer object to return.
		return new Transfer(retrievedTransferNumber, retrievedSourceAccountNumber, retrievedDestinationAccountNumber, retrievedTransferAmt, retrievedDate);
	}

	bool deleteTransfer(int transferNumber)
	{
		// First create a pointer to a SQLiteDatabase using 
		// the connect() function defined above and then
		// create a pointer to an SQLiteStatement object.		
		SQLiteDatabase *pDatabase = this->connect();
		SQLiteStatement *pStmt = this->createStatement(pDatabase);

		// SQL Statement to delete the record
		pStmt->Sql("DELETE FROM AccountTransfer WHERE transferNumber = ?;");
		pStmt->BindInt(1, transferNumber);
		pStmt->ExecuteAndFree();

		// Check to see if any changes have been made to the database
		// due to the SQL statement executed above.  Return the results
		// as a boolean value.
		int recordsDeleted = pDatabase->GetTotalDatabaseChanges();
		if (recordsDeleted)
		{
			// De-allocate memory used to store pointers
			delete pDatabase;
			delete pStmt;
			return true;
		}
		else
		{
			// De-allocate memory used to store pointers
			delete pDatabase;
			delete pStmt;
			return false;
		}
	}

#pragma endregion

// These functions are so similar that they need to be combined somehow.  The objects and 
// database tables for Transfers and Transactions needs to be re-thought and re-designed.
#pragma region Functions related to Creating Transaction and Transfer Histories

	// Function which populates a vector with Page objects.  It uses the accountNumber
	// to find all those TRANSACTIONS for an account, list them in descending order, 
	// build strings with the data, write those strings to a page, and add the page
	// to a "history".
	void populateAccountTransactions(int accountNumber, stack<Transaction> *listOfTransactions)
	{
		// First create a pointer to a SQLiteDatabase using
		// the connect() function defined above and then
		// create a pointer to an SQLiteStatement object.
		SQLiteDatabase *pDatabase = this->connect();
		SQLiteStatement *pStmt = this->createStatement(pDatabase);

		// Declare variables to store the retrieved data
		// from the database.
		int retrievedTransactionNumber = 0;		
		double retrievedTransactionAmt = 0.0;
		string retrievedTransactionType = "";
		string retrievedDate = "";

		// Use the customerNumber and accountType passed to this method to query the database.
		pStmt->Sql("SELECT * FROM AccountTransaction WHERE accountNumber = ? ORDER BY date;");
		pStmt->BindInt(1, accountNumber);

		// Process the results of the query above - assigning the values of each
		// column to the variables declared above.
		while (pStmt->FetchRow())
		{
			retrievedTransactionNumber = pStmt->GetColumnInt("transactionNumber");
			retrievedTransactionAmt = pStmt->GetColumnDouble("transactionAmount");
			retrievedTransactionType = pStmt->GetColumnString("transactionType");
			retrievedDate = pStmt->GetColumnString("date");
		
			Transaction transaction = Transaction(retrievedTransactionNumber, accountNumber, retrievedTransactionAmt, retrievedTransactionType, retrievedDate);

			listOfTransactions->push(transaction);
		}

		// "Clean up"
		pStmt->FreeQuery();	

		// De-allocate memory used to store pointers
		delete pDatabase;
		delete pStmt;
	}

	// Function which populates a vector with Page objects.  It uses the accountNumber
	// to find all those TRANSFERS for an account, list them in descending order, 
	// build strings with the data, write those strings to a page, and add the page
	// to a "history".
	void populateAccountTransfers(int accountNumber, stack<Transfer> *listOfTransfers)
	{
		// First create a pointer to a SQLiteDatabase using
		// the connect() function defined above and then
		// create a pointer to an SQLiteStatement object.
		SQLiteDatabase *pDatabase = this->connect();
		SQLiteStatement *pStmt = this->createStatement(pDatabase);

		// Declare variables to store the retrieved data
		// from the database.
		int retrievedTransferNumber = 0;
		int retrievedSourceAccount = 0;
		int retrievedDestinationAccount = 0;
		double retrievedTransferAmt = 0.0;		
		string retrievedDate = "";

		vector <string> linesToPage;

		// Use the customerNumber and accountType passed to this method to query the database.
		pStmt->Sql("SELECT * FROM AccountTransfer WHERE sourceAccountNumber = ? OR destinationAccount = ? ORDER BY date;");
		pStmt->BindInt(1, accountNumber);
		pStmt->BindInt(2, accountNumber);

		// Process the results of the query above - assigning the values of each
		// column to the variables declared above.
		while (pStmt->FetchRow())
		{
			retrievedTransferNumber = pStmt->GetColumnInt("transferNumber");
			retrievedSourceAccount = pStmt->GetColumnInt("sourceAccountNumber");
			retrievedDestinationAccount = pStmt->GetColumnInt("destinationAccount");
			retrievedTransferAmt = pStmt->GetColumnDouble("transactionAmount");
			retrievedDate = pStmt->GetColumnString("date");

			Transfer newTransfer = Transfer(retrievedTransferNumber, retrievedSourceAccount, retrievedDestinationAccount, retrievedTransferAmt, retrievedDate);

			listOfTransfers->push(newTransfer);			
		}
		
		// "Clean up"
		pStmt->FreeQuery();

		// De-allocate memory used to store pointers
		delete pDatabase;
		delete pStmt;
	}
#pragma endregion
};