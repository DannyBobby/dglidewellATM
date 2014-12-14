#include "UI.h"
#include <iostream>
#include <iomanip>
#include "Windows.h"
#include "Conio.h"

using namespace std;

// Default constructor
UI::UI()
{
}

// Default destructor
UI::~UI()
{
}

// Display returning customer/new customer prompt
bool UI::ShowReturningCustomerNewCustomerPrompt()
{
	// Variables used to display what the 
	// customer currently has selected
	string select1 = "*";
	string select2 = " ";

	// Stores what key has been hit by
	// the customer.
	int keyboardHit = 0;

	// Re-draw the menu as long as the user has
	// not hit the Enter key.
	while (keyboardHit != KEY_ENTER)
	{
		this->ClearScreen();
		cout << endl
			<< "\t\t\t    ***********************" << endl
			<< "\t\t\t    * Welcome to DAT Bank *" << endl
			<< "\t\t\t    ***********************" << endl
			<< endl
			<< endl
			<< " Are you a returning customer?" << endl
			<< endl
			<< "\t[" << select1 << "] 1) Returning customer" << endl   //<---- Choice #1
			<< "\t[" << select2 << "] 2) New customer" << endl;        //<---- Choice #2

		// Get keyboard input from the customer
		keyboardHit = _getch();

		// This switch moves the asterisk amongst the menu
		// items depending on what key has been hit
		switch (keyboardHit)
		{
		case KEY_DOWN:
			if (select1 == "*")
			{
				select1 = " ";
				select2 = "*";
			}
			else if (select2 == "*")
			{
				select2 = " ";
				select1 = "*";
			}			
			break;

		case KEY_UP:
			if (select1 == "*")
			{
				select1 = " ";
				select2 = "*";
			}
			else if (select2 == "*")
			{
				select2 = " ";
				select1 = "*";
			}
			break;
		}
	}

	// After the Enter Key has been pressed, determine
	// what selection was made by which variable has 
	// the asterisk assigned to it.
	if (select1 == "*")
	{
		// This function returns true if the user is a
		// returning customer (See the menu above).
		return true;
	}
	else 
	{
		// This function returns false if the user
		// is not a returning customer.
		return false;
	}		
}

// Display standard login to the user
string UI::ShowLoginPrompt()
{
	// Variable used to store the email 
	// address entered by the customer. 
	string emailInput = "";

	// Show the login screen and prompt 
	// the user for an email address
	this->ClearScreen();
	cout << endl
		<< "\t\t\t    ***********************" << endl
		<< "\t\t\t    * Welcome to DAT Bank *" << endl
		<< "\t\t\t    ***********************" << endl
		<< endl
		<< endl
		<< " Please enter your email: ";

	// Store the input address received from the user.
	cin >> emailInput;
	this->ClearBuffer();

	// Return the email address.
	return emailInput;
}

// If the user is not a returning customer, they need to make an account.
bool UI::ShowCreateNewCustomerProfileForm(Customer *cust)
{
	bool inputValid = false; // <-- Used to determine if the user has validated the input
	int keyboardHit = 0;     // <-- Used to store the value of the key which was hit by the user
	
	// These variables store the pertinent
	// customer info entered by the user.
	string firstName= ""; 
	string lastName = "";
	string emailAddress = "";
	int pin = 0;

	// As long as the customer info has not been validated by the user, loop.
	while (!inputValid)
	{
		// Display a title for the screen and prompt the user for their First Name
		this->ClearScreen();
		cout << endl << "\t\t\t  Create a New Customer Account"
			<< endl << endl << endl << endl
			<< "  Please enter your information below." << endl << endl << endl
			<< "     First Name: ";

		// Store the first name and clear the buffer.
		cin >> firstName;
		this->ClearBuffer();

		// Get the customer's last name.
		cout << "     Last Name: ";
		cin >> lastName;
		this->ClearBuffer();

		// Get the customer's email address.
		do 
		{
			// Prompt for the email address
			cout << "     Email Address: ";
			// Store the input from the user
			cin >> emailAddress;
			// Clear the cin buffer
			this->ClearBuffer();
			// Check to see if the email address
			// is of the correct format.
			if (!ValidateEmail(emailAddress))
			{
				// If the email address isn't valid, redraw the screen
				// with an "*** INVALID EMAIL ADDRESS ***" error message.
				this->ClearScreen();
				cout << endl << "\t\t\t  Create a New Customer Account"
					<< endl << endl << endl << endl
					<< "  Please enter your information below." << endl << endl << endl
					<< "  *** INVALID EMAIL ADDRESS ***" << endl << endl
					<< "     First Name: " + firstName << endl
					<< "     Last Name: " + lastName << endl;
			}
		} while (!ValidateEmail(emailAddress)); // Loop the email prompt until a good email has been entered.

		// Get the customer's PIN.
		do
		{
			// Prompt for the pin
			cout << "     PIN: ";
			// Store the input from the user
			cin >> pin;
			// Clear the cin buffer
			this->ClearBuffer();
			// Check to see if the PIN is of the
			// correct format (4-6 digit integer)
			if (!ValidatePIN(pin))
			{
				// If the PIN isn't valid, redraw the screen with 
				// an "*** INVALID PIN ***" error message.
				this->ClearScreen();
				cout << endl << "\t\t\t  Create a New Customer Account"
					<< endl << endl << endl << endl
					<< "  Please enter your information below." << endl << endl << endl
					<< "  *** INVALID PIN ***" << endl << endl
					<< "     First Name: " + firstName << endl
					<< "     Last Name: " + lastName << endl
					<< "     Email Address: " + emailAddress << endl;
			}
		} while (!ValidatePIN(pin)); // Loop the PIN prompt until a good PIN has been entered.

		// This little bit re-draws the information in the console
		// so that any potential error messages are removed and the 
		// customer can focus on ensuring they entered everything
		// correctly.
		this->ClearScreen();
		cout << endl << "\t\t\t  Create a New Customer Account"
			<< endl << endl << endl << endl
			<< "  Please enter your information below." << endl 
			<< endl << endl
			<< "     First Name: " + firstName << endl
			<< "     Last Name: " + lastName << endl
			<< "     Email Address: " + emailAddress << endl
			<< "     PIN: " + std::to_string(pin);

		// Ask the user to verify the information above as correct.
		cout << endl << endl << endl 
			<< "  Please review the information entered above." << endl		    
			<< endl << endl
			<< "  Is this information correct? (Press \"Y\" for Yes and \"N\" for No)" << endl << endl
			<< endl << endl
			<< "  Press the Escape key to cancel.";

		// Store the value of the key which the customer hit.
		keyboardHit = _getch();
		switch (keyboardHit)
		{
			// 121 is the integer returned by _getch() when the "Y" key is pressed on the keyboard
		case 121:
			// If they hit the "Y" key, change the value of
			// inputValid so that the program exits the loop.
			inputValid = true;
			break;
		case KEY_ESC:
			// If they hit the Escape key, cancel new account creation
			inputValid = true;
			break;
		default:
			inputValid = false;
			break;
		}
	}
	if (keyboardHit == KEY_ESC)
	{
		this->ClearScreen();
		cout << "\n\n\n\n\t\t\t Account Creation Cancelled!\n\n";
		return false;
	}
	else
	{
		cust->SetFirstName(firstName);
		cust->SetLastName(lastName);
		cust->SetEmailAddress(emailAddress);
		cust->SetPIN(pin);
		this->ClearScreen();		
		cout << "\n\n\n\n\t\t\t Account Creation Successful!\n\n";
		return true;
	}
}

// This was taken from "how do i check a user input string with email format" on stackoverflow.com 
// http://stackoverflow.com/questions/14913341/how-do-i-check-a-user-input-string-with-email-format
// this really only checks that it is a normal style email of "email@company.com"
bool UI::ValidateEmail(string const& email)
{
	// Searches for the 1st @ in the input string.
	// The 0 means it searches the entire string for the @ instance
	size_t locationOfAT = email.find_first_of('@', 0);
	// returns true as long as email.find_first_of() found an @ in the string
	// AND if the . is at some location after the the @
	return locationOfAT != std::string::npos && email.find_first_of('.', locationOfAT) != std::string::npos;
}

// Used to check that the user has entered an integer
// between 4 and 6 digits in length (1000 < PIN < 999999)
bool UI::ValidatePIN(int pin)
{
	if (pin >= 1000 && pin <= 999999)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Prompt the user to enter a PIN
int UI::ShowPINPrompt()
{
	int pinInput = NULL;

	cout << endl << endl
		<< " Please enter PIN: ";
	cin >> pinInput;
	this->ClearBuffer();

	return pinInput;
}

// Display the menu of possible transactions to the user as selections 1-6
int UI::ShowTransactionTypeMenu(string custFirstName, string custLastName)
{
	// These "select" variables are used to 
	// drive the menu below.
	char select1 = '*';
	char select2 = ' ';
	char select3 = ' ';
	char select4 = ' ';
	char select5 = ' ';
	char select6 = ' ';
	char select7 = ' ';
	char select8 = ' ';

	// keyboardHit is used to store the value 
	// of the key which was pressed by the user.
	int keyboardHit = 0;

	// Ultimately, this is the value which is returned
	// to the ATM by this function.
	int userSelection = 0;

	// As long as the user hasn't hit the Enter key loop
	while (keyboardHit != KEY_ENTER)
	{
		// Display a menu using the customer's name.
		this->ClearScreen();
		cout << endl
			<< "\t\t\t    Welcome " + custFirstName + " " + custLastName + "!"
			<< endl << endl
			<< "\n\n  Use the UP/DOWN arrow keys to make a selection (1-8):" << endl
			<< endl << endl
			<< "\t[" << select1 << "] 1) Make a withdrawal" << endl          //<---- Choice #1
			<< "\t[" << select2 << "] 2) Make a deposit" << endl             //<---- Choice #2
			<< "\t[" << select3 << "] 3) Check account balance" << endl      //<---- Choice #3
			<< "\t[" << select4 << "] 4) Make a transfer" << endl            //<---- Choice #4
			<< "\t[" << select5 << "] 5) See Transaction History" << endl	 //<---- Choice #5
			<< "\t[" << select6 << "] 6) See Transfer History" << endl       //<---- Choice #6
			<< "\t[" << select7 << "] 7) Manage Account" << endl             //<---- Choice #7
			<< "\t[" << select8 << "] 8) Logout" << endl                     //<---- Choice #7
			<< endl;

		// Get keyboard input from the customer
		keyboardHit = _getch();

		// This switch moves the asterisk amongst the menu
		// items depending on what key has been hit
		switch (keyboardHit)
		{

		// If the down arrow is pressed, move the arrow
		// down - unless it is at the bottom, in that case
		// it moves to the top-most choice.
		case KEY_DOWN:
			if (select1 == '*')
			{
				select1 = ' ';
				select2 = '*';
			}
			else if (select2 == '*')
			{
				select2 = ' ';
				select3 = '*';
			}
			else if (select3 == '*')
			{
				select3 = ' ';
				select4 = '*';
			}
			else if (select4 == '*')
			{
				select4 = ' ';
				select5 = '*';
			}
			else if (select5 == '*')
			{
				select5 = ' ';
				select6 = '*';
			}
			else if (select6 == '*')
			{
				select6 = ' ';
				select7 = '*';
			}
			else if (select7 == '*')
			{
				select7 = ' ';
				select8 = '*';
			}
			else if (select8 == '*')
			{
				select8 = ' ';
				select1 = '*';
			}
			break;

		// If the up arrow is pressed, move the arrow
		// up - unless it is at the top, in that case
		// it moves to the bottom-most choice.
		case KEY_UP:
			if (select1 == '*')
			{
				select1 = ' ';
				select8 = '*';
			}
			else if (select2 == '*')
			{
				select2 = ' ';
				select1 = '*';
			}
			else if (select3 == '*')
			{
				select3 = ' ';
				select2 = '*';
			}
			else if (select4 == '*')
			{
				select4 = ' ';
				select3 = '*';
			}
			else if (select5 == '*')
			{
				select5 = ' ';
				select4 = '*';
			}
			else if (select6 == '*')
			{
				select6 = ' ';
				select5 = '*';
			}
			else if (select7 == '*')
			{
				select7 = ' ';
				select6 = '*';
			}
			else if (select8 == '*')
			{
				select8 = ' ';
				select7 = '*';
			}
			break;
		}
	}

	// After the Enter Key has been pressed, determine
	// what selection was made by which variable has 
	// the asterisk assigned to it.
	if (select1 == '*')
	{
		userSelection = 1;
	}
	else if (select2 == '*')
	{
		userSelection = 2;
	}
	else if (select3 == '*')
	{
		userSelection = 3;
	}
	else if (select4 == '*')
	{
		userSelection = 4;
	}
	else if (select5 == '*')
	{
		userSelection = 5;
	}
	else if (select6 == '*')
	{
		userSelection = 6;
	}
	else if (select7 == '*')
	{
		userSelection = 7;
	}
	else if (select8 == '*')
	{
		userSelection = 8;
	}

	// Return the value of the selection determined by the above if statement.
	keyboardHit = NULL;
	return userSelection;
}

// Display the menu of possible transactions amounts to the user.
double UI::ShowTransactionAmountMenu(char *actionToBePerformed)
{
	// These "amount" variables are used to 
	// drive the menu below.
	char amount1 = '*';
	char amount2 = ' ';
	char amount3 = ' ';
	char amount4 = ' ';
	char amount5 = ' ';

	// keyboardHit is used to store the value 
	// of the key which was pressed by the user.
	int keyboardHit = NULL;

	// Ultimately, this is the value which is returned
	// to the ATM by this function.
	double transactionAmount = 0;
	
	// As long as the user has not hit the Enter key AND they haven't hit the Escape key, loop.
	while (keyboardHit != KEY_ENTER && keyboardHit != KEY_ESC)
	{
		// Display a nice menu for the user to choose from
		this->ClearScreen();
		cout << endl 
			<< "\t\t   Please select amount to be " << actionToBePerformed << ":"
			<< endl << endl << endl << endl
			<< "  Use the UP/DOWN arrow keys to move the selector (*):" << endl
			<< endl << endl
			<< "\t[" << amount1 << "] 1) $20" << endl      //<---- Choice #1
			<< "\t[" << amount2 << "] 2) $40" << endl      //<---- Choice #2
			<< "\t[" << amount3 << "] 3) $60" << endl      //<---- Choice #3
			<< "\t[" << amount4 << "] 4) $80" << endl      //<---- Choice #4
			<< "\t[" << amount5 << "] 5) $100" << endl	  //<---- Choice #5			
			<< endl << endl  
			<< "  Press Enter to make your selection."
			<< endl << endl << endl << endl << endl << endl << endl
			<< "  Press Escape to return to the Main Menu.";

		// Store the value of the key which was hit by the customer
		keyboardHit = _getch();

		// Move the asterisk depending on which key was pressed by the user.
		switch (keyboardHit)
		{

		// If the down arrow is pressed, move the arrow
		// down - unless it is at the bottom, in that case
		// it moves to the top-most choice.
		case KEY_DOWN:
			if (amount1 == '*')
			{
				amount1 = ' ';
				amount2 = '*';
			}
			else if (amount2 == '*')
			{
				amount2 = ' ';
				amount3 = '*';
			}
			else if (amount3 == '*')
			{
				amount3 = ' ';
				amount4 = '*';
			}
			else if (amount4 == '*')
			{
				amount4 = ' ';
				amount5 = '*';
			}
			else if (amount5 == '*')
			{
				amount5 = ' ';
				amount1 = '*';
			}
			break;

		// If the up arrow is pressed, move the arrow
		// up - unless it is at the top, in that case
		// it moves to the bottom-most choice.
		case KEY_UP:
			if (amount1 == '*')
			{
				amount1 = ' ';
				amount5 = '*';
			}
			else if (amount2 == '*')
			{
				amount2 = ' ';
				amount1 = '*';
			}
			else if (amount3 == '*')
			{
				amount3 = ' ';
				amount2 = '*';
			}
			else if (amount4 == '*')
			{
				amount4 = ' ';
				amount3 = '*';
			}
			else if (amount5 == '*')
			{
				amount5 = ' ';
				amount4 = '*';
			}
		}
	}

	// If the Enter key has been hit logic control should have fallen
	// out of the loop and ended up down here.  Determine what value 
	// to return based on "where" the asterisk is.
	if (keyboardHit == KEY_ENTER)
	{
		if (amount1 == '*')
		{
			transactionAmount = 20;
		}
		else if (amount2 == '*')
		{
			transactionAmount = 40;
		}
		else if (amount3 == '*')
		{
			transactionAmount = 60;
		}
		else if (amount4 == '*')
		{
			transactionAmount = 80;
		}
		else if (amount5 == '*')
		{
			transactionAmount = 100;
		}
	}
	
	// Return the amount of the transaction to the calling function.
	return transactionAmount;
}

// Display an area for the customer to input the destination account number for making a transfer
string UI::ShowDestinationAccountPrompt()
{
	// Stores the email address of the account
	// which will receive the transer.
	string email = "";

	// Used to determine how to proceed based on whether the user
	// says the data is accurate or not.
	bool inputValid = false;

	// keyboardHit is used to store the value 
	// of the key which was pressed by the user.
	int keyboardHit = 0;

	// As long as the customer has not certified the accuracy of the email,
	// and they haven't cancelled the transfer using the Escape key... loop
	while (!inputValid && keyboardHit != KEY_ESC)
	{
		// Display the instructions and prompt for input
		this->ClearScreen();
		cout << endl << "\t\t\t  Transfer Funds to Another Account"
			<< endl  << endl << endl << endl
	        << "  Please enter the EMAIL ADDRESS of the account you would like to transfer to." << endl << endl << endl
			<< "  Destination Account: ";

		// Store the user's input for email 
		// and clear the input buffer
		cin >> email;
		this->ClearBuffer();

		// Display the email back to the customer 
		// and ask them to verify its accuracy
		cout << endl << endl << endl << endl
			<< "\t  You entered: " + email 
			<< endl << endl << endl << endl << endl << endl;
		cout << "  Is this correct? (Press Y for \"Yes\" and N for \"No\")" << endl << endl
			<< endl << endl  
			<< "  Press the Escape key to return to the Main Menu.";

		// Store the key which the user hits - we only care if they hit a "Y" or "Esc"
		keyboardHit = _getch();

		// Determine what to do next with that keyboard input
		switch (keyboardHit)
		{
		// 121 is the integer returned by _getch() when the "Y" key is pressed on the keyboard
		case 121:
			
			// If they hit "Y", set inputValid to true 
			// which in turn causes logic to "fall" out
			// of the loop.
			inputValid = true;
			break;
		case KEY_ESC:
			
			// If they hit the "Esc" key, also fall out of the
			// loop by changing inputValid to true (not exactly accurate),
			// but also assign "EscKeyPressedInShowDesAcctPrompt" to email
			// in order to reflect that the user has cancelled the transfer.
			inputValid = true;
			email = "EscKeyPressedInShowDestAcctPrompt";
			break;
			
			// If neither of these keys where hit, loop da loop
		default:
			inputValid = false;
			break;
		}
	}
	
	return email;
}

// Display the account's balance with its own screen
void UI::ShowAccountBalance(string custName, int acctNum, double acctBalance)
{
	this->ClearScreen();
	cout << endl
		<< "\t\t\t\t*******************" << endl
		<< "\t\t\t\t* Current Balance *" << endl
		<< "\t\t\t\t*******************" << endl
		<< endl << endl << endl
		<< " " + custName << endl << endl
		<< "     Account #" + std::to_string(acctNum) + " (Checking): $" << std::fixed << std::setprecision(2) << acctBalance
		<< endl << endl << endl << endl << endl
		<< endl << endl << endl << endl << endl
		<< endl << endl << endl;
	this->PressAnyKeyToContinue();
}

// Transaction Success - feedback to the user that something actually happened
void UI::ShowTransactionSuccessMessage()
{
	this->ClearScreen();
	cout << endl
		<< "\t\t\t   ***********************" << endl
		<< "\t\t\t   * Transaction Success *" << endl
		<< "\t\t\t   ***********************" << endl
		<< endl << endl
		<< "\t\t\tThank you for your business!"
		<< endl << endl << endl << endl << endl << endl;
	this->PressAnyKeyToContinue();
}

// Displays a compile-time-defined message to the user
void UI::ShowErrorMessage(char *message)
{
	this->ClearScreen();
	cout << endl << endl
		<< " *****  " << message << "  *****" << endl
		<< endl << endl << endl << endl << endl << endl;
	this->PressAnyKeyToContinue();
}

// Builds transaction history
vector <Page> UI::BuildTransactionHistory(stack<Transaction> *listOfTransactions)
{
	// Prepare a vector which stores the lines 
	// to be written to the page which is currently
	// "in progress"
	vector <string> linesToPage;

	// This vector is what is built over the course
	// of the function and eventually returned.
	vector <Page> transactionHistory;

	// While their are still lines "waiting to be written" on the stack, loop.
	while (listOfTransactions->size() > 0)
	{
		/******************************************************************************
		 *The following code is all about getting the format correct for each line.
		 ******************************************************************************/

		// We want to work with the transaction on the top of the stack.
		Transaction workingTransaction = listOfTransactions->top();

		// Get the length of the transactionNumber (it could be a single digit, two digits,
		// three digits, etc.)		
		int sizeOfTransactionNumber = std::to_string(workingTransaction.GetTransactionNumber()).length();
		string transactionNumColumnPadding = "";  // string variable to store the "padding"

		// Build a string comprised of spaces.  The number of spaces is determined by the 
		// difference between the "length" of the transaction number (in digits) and the number
		// 6.  Why 6? Because it looks nice.
		for (sizeOfTransactionNumber; sizeOfTransactionNumber < 6; sizeOfTransactionNumber++)
		{
			transactionNumColumnPadding += " ";
		}

		// Format the transactionAmount.  It's currency, so we only want
		// two places after the decimal.
		string retTransAmt = std::to_string(workingTransaction.GetTransactionAmount());  // Turn it into a string
		size_t dotIndex = retTransAmt.find(".");  // Find out where the "." is 
		retTransAmt = retTransAmt.substr(0, dotIndex + 3);  // Trim the string
		int sizeOfTransactionAmt = retTransAmt.length();  // Get the length of the string
		string transactionAmtColumnPadding = "";  // string variable to store the padding.

		// Build a string comprised of spaces in the same we as we did for the transactionNumber
		// column above.
		for (sizeOfTransactionAmt; sizeOfTransactionAmt < 10; sizeOfTransactionAmt++)
		{
			transactionAmtColumnPadding += " ";
		}

		// Build the line by concatenating the various strings created so far.
		string additionalLine = "\t      " + std::to_string(workingTransaction.GetTransactionNumber()) +
			transactionNumColumnPadding + " |  $" +
			transactionAmtColumnPadding +
			retTransAmt + " |  " +
			workingTransaction.GetTransactionType() + "   |  " +
			workingTransaction.GetDate();

		// Add the completed line to the vector representing the
		// collection of lines on the page.
		linesToPage.push_back(additionalLine);

		// Reduce the size of the stack by one
		listOfTransactions->pop();
	}

	// This variable stores the number of lines (transactions)
	// which have been written.
	unsigned int numberOfLinesWritten = 0;

	// As long as there are still lines that need to be written, loop
	while (numberOfLinesWritten < linesToPage.size())
	{
		// Create a new page with room for 10 total lines.
		Page page;

		// While there is still room to write on the page, add an additional line from the vector
		// made above.  Be sure to increment the numberOfLinesWritten as we add to each page.
		for (unsigned int i = 0; i < page.GetMaximumNumberOfLines() && numberOfLinesWritten < linesToPage.size(); i++, numberOfLinesWritten++)
		{
			page.AddLine(linesToPage[numberOfLinesWritten]);
		}

		// Once the maximum number of lines has been reached for the page, add the page to the 
		// transactionHistory vector.
		transactionHistory.push_back(page);
	}

	// Return the newly-made transactionHistory
	return transactionHistory;
}

// Builds transfer history
vector <Page> UI::BuildTransferHistory(stack <Transfer> *listOfTransfers)
{
	// Prepare a vector which stores the lines 
	// to be written to the page which is currently
	// "in progress"
	vector <string> linesToPage;

	// This vector is what is built over the course
	// of the function and eventually returned.
	vector <Page> transferHistory;

	// While their are still lines "waiting to be written" on the stack, loop.
	while (listOfTransfers->size() > 0)
	{
		/******************************************************************************
		*The following code is all about getting the format correct for each line.
		******************************************************************************/

		// We want to work with the transfer on the top of the stack.
		Transfer workingTransfer = listOfTransfers->top();

		// Determine column padding for transferNumber
		int sizeOfNumber = std::to_string(workingTransfer.GetTransactionNumber()).length();
		string transferNumColumnPadding = "";

		// Build the column padding
		for (sizeOfNumber; sizeOfNumber < 7; sizeOfNumber++)
		{
			transferNumColumnPadding += " ";
		}

		// Determine column padding for sourceAccount
		sizeOfNumber = std::to_string(workingTransfer.GetAccountNumber()).length();
		string sourceAccountColumnPadding = "";

		// Build the column padding
		for (sizeOfNumber; sizeOfNumber < 5; sizeOfNumber++)
		{
			sourceAccountColumnPadding += " ";
		}

		// Determine column padding for destinationAccount
		sizeOfNumber = std::to_string(workingTransfer.GetDestinationAccountNumber()).length();
		string destinationAccountColumnPadding = "";

		// Build the column padding
		for (sizeOfNumber; sizeOfNumber < 5; sizeOfNumber++)
		{
			destinationAccountColumnPadding += " ";
		}

		// Format transferAmt output and determine column padding for transferAmt
		string retTransAmt = std::to_string(workingTransfer.GetTransactionAmount());
		size_t dotIndex = retTransAmt.find(".");
		retTransAmt = retTransAmt.substr(0, dotIndex + 3);
		int sizeOfTransferAmt = retTransAmt.length();
		string transferAmtColumnPadding = "";

		// Build the column padding
		for (sizeOfTransferAmt; sizeOfTransferAmt < 10; sizeOfTransferAmt++)
		{
			transferAmtColumnPadding += " ";
		}

		// Build the line item
		string additionalLine = "      " + std::to_string(workingTransfer.GetTransactionNumber()) +
			transferNumColumnPadding + " |  " +
			std::to_string(workingTransfer.GetAccountNumber()) +
			sourceAccountColumnPadding + " |  " +
			std::to_string(workingTransfer.GetDestinationAccountNumber()) +
			destinationAccountColumnPadding + " |  $" +
			transferAmtColumnPadding +
			retTransAmt + " |  " +
			workingTransfer.GetDate();

		// Add the line item to the list of lines to be written in the report.
		linesToPage.push_back(additionalLine);

		// Reduce the size of the stack by one
		listOfTransfers->pop();
	}

	// This variable stores the number of lines (transactions)
	// which have been written.
	unsigned int numberOfLinesWritten = 0;

	// As long as there are still lines that need to be written, loop
	while (numberOfLinesWritten < linesToPage.size())
	{
		// Create a new page with room for 10 total lines.
		Page page;

		// While there is still room to write on the page, add an additional line from the vector
		// made above.  Be sure to increment the numberOfLinesWritten as we add to each page.
		for (unsigned int i = 0; i < page.GetMaximumNumberOfLines() && numberOfLinesWritten < linesToPage.size(); i++, numberOfLinesWritten++)
		{
			page.AddLine(linesToPage[numberOfLinesWritten]);
		}

		// Once the maximum number of lines has been reached for the page, add the page to the 
		// transactionHistory vector.
		transferHistory.push_back(page);
	}

	// Return the newly-made transferHistory
	return transferHistory;	
}

// Function to display transaction history
void UI::ShowTransactionHistory(vector<Page> transactionHistory, string firstName, string lastName)
{
	// Variable used to determine what 
	// page the user is currently viewing
	unsigned int currentPage = 0;

	// Store keyboard input
	int keyboardHit = 0;

	// As long as the user has not hit the Escape key, loop
	while (keyboardHit != KEY_ESC)
	{
		// Display the column headers
		this->ClearScreen();
		cout << endl << " Transaction History for " + firstName + " " + lastName << endl << endl << endl;
		cout << "\t     Number  |    Amount    | Type |         Date" << endl;
		cout << "\t    -----------------------------------------------------" << endl;

		// If the size of the trasactionHistory vector is greater than zero,
		// proceed to print each line of the current page onto the screen.
		if (transactionHistory.size() > 0)
		{
			// Call the GetLine() function for each line on the page and print the string to the console.
			for (unsigned int i = 0; i < transactionHistory[currentPage].GetNumberOfLines(); i++)
			{
				cout << transactionHistory[currentPage].GetLine(i) << endl;
			}
			// If the number of lines stored in the page is less than the maximum amount,
			// make sure to print a number of blank lines equal to the difference between
			// the maximum number and the number of lines on the page.  This is to ensure
			// consistency between the layout of each page of the history printed to the 
			// console regardless of the number of lines on the page.
			if (transactionHistory[currentPage].GetNumberOfLines() < transactionHistory[currentPage].GetMaximumNumberOfLines())
			{
				// Find the difference between the maximum number of lines possible for a page
				// and the number of lines stored on the page.
				unsigned int numberOfBlankLines = transactionHistory[currentPage].GetMaximumNumberOfLines() - transactionHistory[currentPage].GetNumberOfLines();

				// Print blank lines
				for (unsigned int i = 0; i < numberOfBlankLines; i++)
				{
					cout << endl;
				}
			}
		}
		// If the size of the transaction history is zero, there are no lines to print.
		// This is definitely possible for new customer.  Print 10 blank lines to the 
		// screen (10 is the maximum number of lines per page and in keeping with the 
		// format of the history screen.
		else
		{
			for (unsigned int i = 0; i < 10; i++)
			{
				cout << endl;
			}
		}		

		// Print some blank lines to make it look pretty.
		cout << endl << endl << endl << endl;

		// These are instructions for moving between each page of the history; left and right.
		cout << " <--- Previous Page (Left Arrow)\t\t   Next Page (Right Arrow) --->" << endl << endl << endl;
		cout << " Press Escape key to return to Main Menu";

		// Get the keyboard input
		keyboardHit = _getch();

		// Determine what to do with that keyboard input
		switch (keyboardHit)
		{
		// If the user hit the left arrow...
		case KEY_LEFT:
			// ...and if the index of currentPage 
			// being viewed is greater than zero...
			if (currentPage > 0)
			{
				// ...decrement the currentPage counter;
				// effectively turning the page backwards.
				currentPage--;
			}
			break;
		// If the user hit the right arrow...
		case KEY_RIGHT:
			// ...and if the index of currentPage being viewed
			// is less than the size of transactionHistory minus 1...
			if (currentPage < (transactionHistory.size() - 1))
			{
				// Increment the currentPage counter;
				// effectively moving forwards through 
				// the history.
				currentPage++;
			}
		}
	}
	
}

// Another history function, this time for transfers - they look slightly different
void UI::ShowTransferHistory(vector<Page> transferHistory, string firstName, string lastName)
{
	// Variable used to determine what 
	// page the user is currently viewing
	unsigned int currentPage = 0;

	// Store keyboard input
	int keyboardHit = 0;

	// As long as the user has not hit the Escape key, loop
	while (keyboardHit != KEY_ESC)
	{
		// Display the column headers
		this->ClearScreen();
		cout << endl << " Transfer History for " + firstName + " " + lastName << endl << endl << endl;
		cout << "     Transfer | Source |  Dest. |   Transfer   |" << endl;
		cout << "      Number  |  Acct  |  Acct  |    Amount    |         Date" << endl;
		cout << "    ------------------------------------------------------------------" << endl;

		// If the size of the trasactionHistory vector is greater than zero,
		// proceed to print each line of the current page onto the screen.
		if (transferHistory.size() > 0)
		{
			// Call the GetLine() function for each line on the page and print the string to the console.
			for (unsigned int i = 0; i < transferHistory[currentPage].GetNumberOfLines(); i++)
			{
				cout << transferHistory[currentPage].GetLine(i) << endl;
			}

			// If the number of lines stored in the page is less than the maximum amount,
			// make sure to print a number of blank lines equal to the difference between
			// the maximum number and the number of lines on the page.  This is to ensure
			// consistency between the layout of each page of the history printed to the 
			// console regardless of the number of lines on the page.
			if (transferHistory[currentPage].GetNumberOfLines() < transferHistory[currentPage].GetMaximumNumberOfLines())
			{
				// Find the difference between the maximum number of lines possible for a page
				// and the number of lines stored on the page.
				unsigned int numberOfBlankLines = transferHistory[currentPage].GetMaximumNumberOfLines() - transferHistory[currentPage].GetNumberOfLines();

				// Print blank lines.
				for (unsigned int i = 0; i < numberOfBlankLines; i++)
				{
					cout << endl;
				}
			}
		}
		// If the size of the transaction history is zero, there are no lines to print.
		// This is definitely possible for new customer.  Print 10 blank lines to the 
		// screen (10 is the maximum number of lines per page and in keeping with the 
		// format of the history screen.
		else
		{
			for (unsigned int i = 0; i < 10; i++)
			{
				cout << endl;
			}
		}

		// Print some blank lines to make it look pretty.
		cout << endl << endl << endl << endl;

		// These are instructions for moving between each page of the history; left and right.
		cout << " <--- Previous Page (Left Arrow)\t\t   Next Page (Right Arrow) --->" << endl << endl << endl;
		cout << " Press Escape key to return to Main Menu";

		// Get the keyboard input
		keyboardHit = _getch();
		switch (keyboardHit)
		{
		// If the user hit the left arrow...
		case KEY_LEFT:
			// ...and if the index of currentPage 
			// being viewed is greater than zero...
			if (currentPage > 0)
			{
				// ...decrement the currentPage counter;
				// effectively turning the page backwards.
				currentPage--;
			}
			break;
		// If the user hit the right arrow...
		case KEY_RIGHT:
			// ...and if the index of currentPage being viewed
			// is less than the size of transactionHistory minus 1...
			if (currentPage < (transferHistory.size() - 1))
			{
				// Increment the currentPage counter;
				// effectively moving forwards through 
				// the history.
				currentPage++;
			}
		}
	}
}

// Displays a prompt for the user to use if they want to "close" their account
bool UI::ShowChangeAccountStatusPrompt()
{
	// Prompt the user: Do you want to make your account inactive?
	this->ClearScreen();
	cout << endl << endl << endl << "  Make Account Inactive? (Y/N)";

	// Find out what key they pressed
	int key = _getch();

	// If they hit "Y" (_getch() returns 121 when "Y" is pressed)
	if (key == 121)
	{
		// Ask the user again, are you really really sure?
		cout << endl << endl << endl << endl << endl << endl << endl << endl
			<< "  Are you absolutely sure you want to make this account INACTIVE?" 
			<< endl << endl << endl << endl
			<< "  You will be logged out immediately following this action. (Y/N)";

		// Find out what they pressed
		key = _getch();

		// If they pressed "Y" again...
		if (key == 121)
		{
			// return true, which signals to the calling function that
			// the customer does indeed intend to close the account.
			return true;
		}
		// Otherwise, don't do anything because the 
		// customer changed their mind.
		else
			return false;
	}		
	// Otherwise, don't do anything because the 
	// customer changed their mind.
	else
		return false;	
}

// Clear the screen - THIS CODE IS NOT MY OWN.
// I created the ClearScreen function using 
// code from http://www.cplusplus.com/articles/4z18T05o/
void UI::ClearScreen()
{
	HANDLE hStdout;
	COORD coordScreen = { 0, 0 };    // home for the cursor 
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	// Get the number of character cells in the current buffer. 

	if (!GetConsoleScreenBufferInfo(hStdout, &csbi))
		return;
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

	// Fill the entire screen with blanks.

	if (!FillConsoleOutputCharacter(hStdout, (TCHAR) ' ',
		dwConSize, coordScreen, &cCharsWritten))
		return;

	// Get the current text attribute.

	if (!GetConsoleScreenBufferInfo(hStdout, &csbi))
		return;

	// Set the buffer's attributes accordingly.

	if (!FillConsoleOutputAttribute(hStdout, csbi.wAttributes,
		dwConSize, coordScreen, &cCharsWritten))
		return;

	// Put the cursor at its home coordinates.

	SetConsoleCursorPosition(hStdout, coordScreen);
}

// Pause the screen
void UI::PauseScreen()
{
	while (!_kbhit()){}
}

// Pause the screen, Display "Press any key..." and wait for 
// the user to strike a key.
void UI::PressAnyKeyToContinue()
{
	cout << "  Press any key to continue...";
	while (!_kbhit()){}
	char key = _getch();
}

/**************** CODE TO CLEAR THE INPUT BUFFER ****************
****************************************************************/
void UI::ClearBuffer()
{
	cin.clear();
	cin.ignore(INT_MAX, '\n');
}

bool UI::PromptATMShutdown()
{
	// Prompt the user whether they would like to close the program.
	cout << "\n\n\n\n\t\t       Press any other key to continue." << endl << endl
		<< "\t\t Strike the \" Esc \" key to close the program.";

	// Get the integer value corresponding to the key that they struck
	int key = _getch();

	// If the user has struck the "Esc" key, they intend on closing the program.
	if (key == KEY_ESC) // 27 is the integer returned by the Escape key
	{
		return true;
	}
	else
		return false;
}