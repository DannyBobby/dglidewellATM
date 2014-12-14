#ifndef UI_H
#define UI_H

#include <string>
#include <vector>
#include "Page.h"
#include "Customer.h"

using namespace std;

// These are preprocessor directives used in 
// the logic which facilitates menu navigation. 
// Inspiration for this code and the code in the 
// switches which use these directives was found at:
// http://www.stackoverflow.com/questions/24708700/c-detect-when-user-presses-arrow-key
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ENTER 13
#define KEY_ESC 27

// The Display object represents the "screen" of the UI
class UI
{

public:
	// Constructor & destructor
	UI();
	~UI();

	// All of these functions print things to the screen.
	// The function names are pretty self-explanatory.
	bool ShowReturningCustomerNewCustomerPrompt();
	bool ShowCreateNewCustomerProfileForm(Customer *cust);
	string ShowLoginPrompt();
	int ShowPINPrompt();

	// This function displays a menu to the customer with the possible
	// types of transactions they can perform:
	//       Withdrawal
	//       Deposit
	//       View Transaction History
	//       ...
	int ShowTransactionTypeMenu(string custFirstName, string custLastname);

	// This functions displays a menu to the customer with the possible
	// amounts which can be withdrawn, deposited, transferred, etc.
	double ShowTransactionAmountMenu(char *actionToBePerformed);

	// This function is used for Transfers.  
	// It prompts the user for the account to which
	// the transfer will be sent and also provides a
	// way for the user to "back out" of the transaction.
	string ShowDestinationAccountPrompt();

	// These function names are pretty self-explanatory
	void ShowAccountBalance(string custName, int acctNum, double acctBalance);
	void ShowTransactionSuccessMessage();
	void ShowErrorMessage(char *message);

	// These functions are so similar that they really should be consolidated.
	void ShowTransactionHistory(vector<Page> transactionHistory, string firstName, string lastName);
	void ShowTransferHistory(vector<Page> transferHistory, string firstName, string lastName);

	// Display instructions for changing the status of an account
	bool ShowChangeAccountStatusPrompt();

	// These functions are frequently used
	// to manipulate output to the screen.
	void ClearScreen();
	void PauseScreen();
	void PressAnyKeyToContinue(); //<--- Pauses screen and waits for a key press
	void ClearBuffer();           //<--- Used to clear the "cin buffer" - thanks C++!
};

#endif