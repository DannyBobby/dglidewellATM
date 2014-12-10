#include "ATM.h"
#include <iostream>
#include <vector>

using namespace std;

// UI object used to interface with the customer
// This is necesary to keep the business logic 
// separate from the UI.
UI *ui = new UI();

// Here are the global Customer and Account objects for all ATM functions.
// This made more sense to me rather than passing these objects
// back and forth between the various functions.
Customer *customer;
Account *account;

// This database object is used to communicate with the embedded SQLite database.
Database *db = new Database();

// This vector is used to store all the transactions that the customer
// performs while they are logged in.  Transactions and account updates 
// are made once the user logs out.
std::vector<Transaction> collectionOfTransactions;

// default ATM Constructor
ATM::ATM()
{
}

// default ATM Destructor
ATM::~ATM()
{
}

bool ATM::Login() {
	// stores the email addresss and PIN entered by the user	

	// prompts the User to enter their email
	email = ui->ShowLoginPrompt();
	ui->ClearBuffer();

	// prompts the user to enter their PIN, without clearing the LoginPrompt
	pin = ui->ShowPINPrompt();
	ui->ClearBuffer();

#pragma region Authentication logic
	customer = db->getCustomer(email);

	// Test to see if a valid Customer was returned by the getCustomer()
	// function call above.
	if (customer->GetCustomerNumber() != 0)
	{
		// If the PIN entered by the user matches the one that is stored
		// in the database, get the account information for that customer and 
		// return "true" so that the MainMenu is called (see the Source.cpp file)
		if (customer->GetPIN() == pin)
		{		
			account = db->getAccount(customer->GetCustomerNumber());
			return true;
		}
		else
			// Return false if the PINs do no match.
			return false;
	}
	else
		return false;
#pragma endregion
}

// MainMenu method
// uses a switch to determine what the user would like to do during this interaction.
void ATM::MainMenu() {

	bool userLogout = false;
	
	do{
		// input from the user used to control the switch			
		int actionToBePerformed = 0;
		
		actionToBePerformed = ui->ShowTransactionTypeMenu(customer->GetFirstName(), customer->GetLastName());

		// calls the users desired interaction based on the user input
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
			//transfer();
			cout << "Perform Transfer" << endl << endl;
			system("pause");
			break;
		case 5:
			//history();
			cout << "Perform Transaction History" << endl << endl;
			system("pause");
			break;
		case 6: 
			LogoutCustomer();
			userLogout = true;			
			break;
		}
	} while (!userLogout);
}

// This logic executes if the user selected to see their balance from the Main Menu
void ATM::PerformBalanceInquiry() {

	// Build a string to show the customer's name.
	string customerName = customer->GetFirstName() + " " + customer->GetLastName();

	// Show the current balance of the account.
	ui->ShowAccountBalance(customerName, account->GetAccountBalance());
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

	// Check to see if the customer has the available funds to perform the withdrawal
	if (accountBalance >= amountToWithdraw)
	{
		// Create a new Transaction object and add it to the "batch" of transactions to process 
		// when the user chooses to log out.
		Transaction newWithdrawal = Transaction(account->GetAccountNumber(), amountToWithdraw, "W");
		collectionOfTransactions.push_back(newWithdrawal);

		// Add the amount of the new deposit to the account's balance
		accountBalance -= amountToWithdraw;
		account->SetAccountBalance(accountBalance);

		// Show the user that the transaction was a success.
		ui->ShowTransactionSuccessMessage();
	}
	else
		// If the user does not have sufficient funds to cover the withdrawal, display this error message.
		ui->ShowErrorMessage("There are insufficient funds in this account to perform a withdrawal of that amount.");
}

// This logic executes if the user selected to make a deposit from the Main Menu
void ATM::PerformDeposit() 
{
	double accountBalance = account->GetAccountBalance();
	// At this point, the user will be shown the transaction menu where they 
	// will make a selection.  The amount which they would like to deposit
	// is then returned and stored in the "amountToDeposit" variable declared below.
	double amountToDeposit = ui->ShowTransactionAmountMenu("deposited");

	// Create a new Transaction object and add it to the "batch" of transactions to process 
	// when the user chooses to log out.
	Transaction newDeposit = Transaction(account->GetAccountNumber(), amountToDeposit, "D");
	collectionOfTransactions.push_back(newDeposit);

	// Add the amount of the new deposit to the account's balance
	accountBalance += amountToDeposit;
	account->SetAccountBalance(accountBalance);

	// Show the user that the transaction was a success.
	ui->ShowTransactionSuccessMessage();
}

// This logic executes if the user selected to make a transfer from the Main Menu
int ATM::PerformTransfer() {
	// takes in the account the user would like to transfer money to and the amount to transfer
	char accountToTransferTo[50] = "NULL";
	int amountToTransfer = NULL;

	// prints a preconstructed transaction amounts menu (forces increments of $20 for shallow prototype)
	// has a clear screen function inside that clears everything that happened on the MainMenu
	ui->ShowTransactionAmountMenu("transfered");
	// takes in the amount the user would like to transfer
	cin >> amountToTransfer;

	ui->ClearBuffer();

	// asks the user for the email of the account to transfer money to
	cout << "Please enter the Account you would like to transfer to: ";
	// takes in the email of the account to transfer money to
	cin >> accountToTransferTo;
	
	ui->ClearBuffer();

	// again for the shallow prototype, there will only be success, 
	// so this screen will automatically be displayed next
	ui->ShowTransactionSuccessMessage();

	// recalls MainMenu, meaning the screen is cleared of everything
	// that happened during deposit
	return 0;
}

// This logic executes if the user selected to view their transaction history from the Main Menu
void ATM::ShowTransactionHistory() 
{
	
}

// logoutCustomer() writes all the new transactions (if any) to the database
// and performs any other house-cleaning routines.
void ATM::LogoutCustomer()
{
	// Move through the collection of Transactions and write each one to the database.
	// This could be more efficient if we employed a batch process rather than writing
	// each transaction one at a time.
	for (unsigned int i = 0; i < collectionOfTransactions.size(); i++)
	{
		db->createTransaction(collectionOfTransactions[i].GetAccountNumber(),
							  collectionOfTransactions[i].GetTransactionAmount(),
							  collectionOfTransactions[i].GetTransactionType());
	}	

	// Update the account information which is stored in the database to reflect 
	// the account's new balance after all the transactions have taken place.
	db->updateBalance(account->GetAccountNumber(), account->GetAccountBalance());

	// Clear out the vector keeping track of the customer's transactions
	collectionOfTransactions.clear();
}