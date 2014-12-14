#include "ATM.h"
#include <iostream>
#include <vector>

using namespace std;

#pragma region Objects instantiated for use by the ATM

// UI object used to interface with the customer
// This is necesary to keep the business logic 
// separate from the UI.
UI *ui = new UI();

// Here are the global Customer and Account objects for all ATM functions.
// This made more sense to me rather than passing these objects
// back and forth between the various functions.
Customer *customer = new Customer();
Account *account;

// This database object is used to communicate with the embedded SQLite database.
Database *db = new Database();

// This vector is used to store Page objects which facilitate the
// display of the transactionHistory of an account.
std::vector<Page> history;

// This stack is used to store the list of TRANSACTIONS which
// are used to create a transaction history for the customer.
std::stack<Transaction> listOfTransactions;

// This stack is used to store the list of TRANSFERS which
// are used to create a transaction history for the customer.
std::stack<Transfer> listOfTransfers;

#pragma endregion

// default ATM Constructor
ATM::ATM()
{
}

// default ATM Destructor
ATM::~ATM()
{
}

bool ATM::Login() {

	// Prompt the user: Are you a returning customer or a new customer?
	// Store the answer and determine how to proceed below.
	bool returningCustomer = ui->ShowReturningCustomerNewCustomerPrompt();

	// If they are a returning customer, proceed with normal login procedure.
	if (returningCustomer)
	{
		// prompts the User to enter their email
		email = ui->ShowLoginPrompt();

		// prompts the user to enter their PIN
		pin = ui->ShowPINPrompt();

		// Get the customer data from the database using the email address provided
		customer = db->getCustomer(email);

		// Test to see if a valid Customer was returned by the getCustomer()
		// function call above. A customer number of 0 is not possible...
		if (customer->GetCustomerNumber() != 0)
		{
			// If the PIN entered by the user matches the one that is stored
			// in the database, get the account information for that customer and 
			// return "true" so that the MainMenu is called (see the Source.cpp file)
			if (customer->GetPIN() == pin)
			{
				// We've hard-coded a "C" here because we built the function with 
				// the idea that in the future customers could have more than one
				// type of account.
				account = db->getAccount(customer->GetCustomerNumber(), "C");

				// Check to see if the account is "ACTIVE" or "INACTIVE".  
				// If it's active, proceed.
				if (account->GetAccountStatus() == "ACTIVE")
				{
					return true;
				}
				// If it's not active, give an error message and proceed no further.
				else
				{
					ui->ShowErrorMessage("This account is currently inactive!");
					return false;
				}				
			}
			// Return false if the PINs do no match.
			else
			{
				ui->ShowErrorMessage("Invalid email address or PIN.");
				return false;
			}
				
		}
		// If a bad customer email was given and the database could
		// not build a customer object from the data, it should return 
		// a customer number of zero.  Proceed no further.
		else
		{
			ui->ShowErrorMessage("Invalid email address or PIN.");
			return false;
		}			
	}
	// If the user is not a returning customer, they must 
	// have chosen to create a new customer profile.
	else
	{
		CreateNewCustomer();
		return false;
	}
}

// Called during the Login() function if the user chooses to create a new account
void ATM::CreateNewCustomer()
{
	// This boolean value is true only if the ShowCreateNewCustomerProfile function
	// call below returns true.  This indicates that the user did not cancel the 
	// new customer creation process.
	bool proceedWithCustomerCreation = false;

	// Show the screen which walks the customer through creating a new account.
	// The data that the customer enters is stored in the customer object.
	proceedWithCustomerCreation = ui->ShowCreateNewCustomerProfileForm(customer);

	// If the customer has chosen to create an account, proceed.
	if (proceedWithCustomerCreation)
	{
		// Using the customer object which was altered in the ui function call above,
		// create a new record in the customer table of the database.
		db->createCustomer(customer->GetLastName(), customer->GetFirstName(), customer->GetEmailAddress(), customer->GetPIN());

		// This function call to the database's getCustomer() function is necessary because the 
		// subsequent createAccount()function uses the customerNumber to create an account.
		// The customer number is assigned by the DBMS, so we have to re-create the customer 
		// object after the record has been made in order to get that customerNumber.
		customer = db->getCustomer(customer->GetEmailAddress());
		
		// Hard-coded a "C" for checking account type.
		// Clearly this can and should change in the future.
		db->createAccount(customer->GetCustomerNumber(), "C");  
	}
}

// The Main Menu uses a switch to determine what the user would like to do during this interaction.
void ATM::MainMenu() {

	bool userLogout = false;
	
	// Used a do-while because invariably this logic will be called at least once.
	do{
		// This variable stores the integer returned by the UI's
		// ShowTransactionTypeMenu() function.  The integer is
		// used to determine what ATM-related action to perform.
		int actionToBePerformed = 0;
		
		// Display the list of possible transactions available to the customer and 
		// return the customer's choice to the actionToBePerformed variable.
		actionToBePerformed = ui->ShowTransactionTypeMenu(customer->GetFirstName(), customer->GetLastName());
		
		// Uses the result from above to call the function
		// related to the user's desired transaction.
		// See the ui function ShowTransactionTypeMenu() for
		// more clarification.
		switch (actionToBePerformed)
		{
		case 1:
			PerformWithdrawal();
			break;
		case 2:
			PerformDeposit();
			break;
		case 3:
			PerformBalanceInquiry();
			break;
		case 4:
			PerformTransfer();			
			break;
		case 5:
			ShowTransactionHistory();
			break;
		case 6:
			ShowTransferHistory();
			break;
		case 7: 
			ManageAccount();
			break;
		case 8:
			LogoutCustomer();
			userLogout = true;
			break;
		}
	// Do not break the loop until either the user has chosen to 
	// log out or they have closed their account, making it INACTIVE.
	} while (!userLogout && account->GetAccountStatus() == "ACTIVE");
}

// This logic executes if the user selected to see their balance from the Main Menu
void ATM::PerformBalanceInquiry() {

	// Build a string to show the customer's name.
	string customerName = customer->GetFirstName() + " " + customer->GetLastName();

	// Show the current balance of the account.
	ui->ShowAccountBalance(customerName, account->GetAccountNumber(), account->GetAccountBalance());
}

// This logic executes if the user selected to make a withdrawal from the Main Menu
void ATM::PerformWithdrawal() 
{
	// Get the current balance of the account
	double accountBalance = account->GetAccountBalance();

	// At this point, the user will be shown the transaction menu where they 
	// will make a selection.  The amount which they would like to withdraw
	// is then returned and stored in the "amountToWithdraw" variable declared below.
	double amountToWithdraw = ui->ShowTransactionAmountMenu("withdrawn");

	// Check to see if the customer has the available funds to perform the withdrawal.
	// There is a comparison of amountToWithdraw to 0 because ShowTransactionAmountMenu()
	// returns 0 if the user has hit the Escape key; cancelling the transaction.
	if (amountToWithdraw != 0 && account->GetAccountBalance() >= amountToWithdraw)
	{
		// Subtract the amount of the withdrawal from the account's balance
		accountBalance -= amountToWithdraw;
		account->SetAccountBalance(accountBalance);		

		// Record the transaction in the database.
		db->createTransaction(account->GetAccountNumber(), amountToWithdraw, "W");
		ui->ShowTransactionSuccessMessage();
	}
	else if (amountToWithdraw != 0 && account->GetAccountBalance() < amountToWithdraw)
	{
		// If the user does not have sufficient funds to cover the withdrawal, display this error message.
		ui->ShowErrorMessage("Insufficient funds in this account!");
	}		
}

// This logic executes if the user selected to make a deposit from the Main Menu
void ATM::PerformDeposit() 
{
	double accountBalance = account->GetAccountBalance();
	// At this point, the user will be shown the transaction menu where they 
	// will make a selection.  The amount which they would like to deposit
	// is then returned and stored in the "amountToDeposit" variable declared below.
	double amountToDeposit = ui->ShowTransactionAmountMenu("deposited");

	// As with the withdrawal function abovve, ShowTransactionAmountMenu will return
	// 0 if the customer hits the Escape key instead of choosing an amount.  So if
	// the user didn't hit the Escape key, proceed.
	if (amountToDeposit != 0)
	{
		// Add the amount of the new deposit to the account's balance
		accountBalance += amountToDeposit;
		account->SetAccountBalance(accountBalance);

		// Record the transaction in the database.
		db->createTransaction(account->GetAccountNumber(), amountToDeposit, "D");
		ui->ShowTransactionSuccessMessage();
	}	
}

// This logic executes if the user selected to make a transfer from the Main Menu
// A transfer is the movement of funds from one account to another.
void ATM::PerformTransfer() {

	// Variable declaration and initiation
	double currentBalanceOfSourceAccount = account->GetAccountBalance();
	string destCustomerEmail = "";
	double amountToTransfer = 0.0;

	// Get the number for the account which will be recieving the transfer and
	// use that number to retrieve that account's information - building an
	// Account object named destinationAccount.
	destCustomerEmail = ui->ShowDestinationAccountPrompt();
	Customer *destinationCustomer = db->getCustomer(destCustomerEmail);
		
	if ((destinationCustomer->GetCustomerNumber() != 0) && (destCustomerEmail != "EscKeyPressedInShowDestAcctPrompt") && (destCustomerEmail != customer->GetEmailAddress()))
	{
		Account *destinationAccount = db->getAccount(destinationCustomer->GetCustomerNumber(), "C");
		// Prints a preconstructed transaction amounts menu (forces increments of $20)
		// and stores the result of the user's choice in the amountToTransfer variable.
		amountToTransfer = ui->ShowTransactionAmountMenu("transferred");

		if (currentBalanceOfSourceAccount >= amountToTransfer)
		{
			// Subtract the amount of the new transfer from the "source" account's balance
			currentBalanceOfSourceAccount -= amountToTransfer;
			account->SetAccountBalance(currentBalanceOfSourceAccount);
						
			// And add the amount of the new transfer to the "destination" account's balance
			double BalOfDestAcct = destinationAccount->GetAccountBalance();
			BalOfDestAcct += amountToTransfer;
			destinationAccount->SetAccountBalance(BalOfDestAcct);
						
			// Update the Destination Account's Balance
			db->updateBalance(destinationAccount->GetAccountNumber(), destinationAccount->GetAccountBalance());

			// Update the Transfer Table in the database
			db->createTransfer(account->GetAccountNumber(), destinationAccount->GetAccountNumber(), amountToTransfer);

			// Show that the transaction was a success
			ui->ShowTransactionSuccessMessage();

			// Delete this object as it is only used 
			// in the context of a transfer.
			delete destinationAccount;
		}
		else
			// If the user does not have sufficient funds to cover the withdrawal, display this error message.
			ui->ShowErrorMessage("Insufficient funds in this account!");
	}
	// If the user has entered a bad email, resulting in null 
	// values being returned to the destinationCustomer object,
	// and they have NOT pressed the Escape key...
	else if (destinationCustomer->GetCustomerNumber() == 0 && destCustomerEmail != "EscKeyPressedInShowDestAcctPrompt")
	{
		// Display this handy error message.
		ui->ShowErrorMessage("No account exists for that email address.");
	}	
	// If the customer has entered the email for the account
	// they are currently logged in as...
	else if (destCustomerEmail == customer->GetEmailAddress())
	{
		// Display this handy error message.
		ui->ShowErrorMessage("Cannot transfer to an account while it is being used.");
	}

	// Delete this object as it is only used 
	// in the context of a transfer.
	delete destinationCustomer;
}

// This logic executes if the user selected to view their TRANSACTION history from the Main Menu
// I would like to consolidate the ShowTransactionHistory() and ShowTransferHistory() functions
// because this code is extremely redundant.
void ATM::ShowTransactionHistory() 
{
	// Call upon the database and send it the account number and a stack which
	// will be used to store the transactions tied to this account.
	db->populateAccountTransactions(account->GetAccountNumber(), &listOfTransactions);

	// Use the UI to to build and display a transaction history along with the customer's name.
	ui->ShowTransactionHistory(ui->BuildTransactionHistory(&listOfTransactions), customer->GetFirstName(), customer->GetLastName());
}

// This logic executes if the user selected to view their TRANSFER history from the Main Menu.
// I would like to consolidate the ShowTransferHistory() and ShowTransactionHistory() functions
// because this code is extremely redundant.
void ATM::ShowTransferHistory()
{	
	// Call upon the database and send it the account number and a stack which 
	// will be used to store the transfers tied to this account.
	db->populateAccountTransfers(account->GetAccountNumber(), &listOfTransfers);
	
	// Use the UI to build and display a transfer history along with the customer's name.
	ui->ShowTransferHistory(ui->BuildTransferHistory(&listOfTransfers), customer->GetFirstName(), customer->GetLastName());
}

// This logic executes if the user selected to manage their account from the Main Menu
void ATM::ManageAccount()
{
	// Call a UI function which displays a prompt to change 
	// the account status and returns a boolean value.
	bool changeStatus = ui->ShowChangeAccountStatusPrompt();
	
	// If the boolean value is true...
	if(changeStatus)
	{
		// set the status of the account to INACTIVE...
		account->SetAccountStatus("INACTIVE");
		// and update the database.
		db->updateStatus(account->GetAccountNumber(), account->GetAccountStatus());
	}
}

// logoutCustomer() writes all the new transactions (if any) to the database
// and performs any other house-cleaning routines.
void ATM::LogoutCustomer()
{
	// Update the currently logged-in user's account information stored in the database
	// to refelct the account's new balance after all the transactions have taken place.
	db->updateBalance(account->GetAccountNumber(), account->GetAccountBalance());

	// Clear out the vectors keeping track of the "pages" of the account's histories.	
	history.clear();
}

// This function simply cleans up any pointers that would
// otherwise be left lying around after shutdown.
void ATM::ShutdownATM()
{
	// Delete those objects which were
	// instantiated on the heap.
	delete customer;
	delete account;
	delete ui;
	delete db;
}