// Danny Glidewell, Tucker Lavell, Anthony Tran, Joshua Blanton, Ian Renninger
// TR 5:30-6:45
// Assignment: ATM Revisited

#include <iostream>
#include <cmath>
#include "Windows.h"
#include "Conio.h"
#include "UI.h"
#include "ATM.h"
#include "Database.h"
#include "Customer.h"

using namespace std;

int main() {
	// Boolean value for determining when to shutdown the ATM
	bool atmShutdown = false;

	// Create our ATM object and our UI object
	ATM *atm = new ATM();
	UI *mainUI = new UI();

	do
	{
		// If the user has successfully logged in...
		if (atm->Login())
		{
			// ...Show them the Main Menu
			atm->MainMenu();
		}

		// The Main Menu is a loop, so once they choose to logout, execution picks
		// up here.  This next bit is included in this program in order to give us the 
		// option of turning off the program.  It doesn't make much sense for a production
		// ATM, but for our purposes it works just fine.

		// Determine if the user wants to exit the program.
		atmShutdown = mainUI->PromptATMShutdown();
				
	} while (!atmShutdown);		// Keep looping until the user decides to exit the program

	// Once the user decides to exit the program and the 
	// loop is broken, call the ShutdownATM() function 
	// which cleans up any objects which may still be 
	// on the heap.
	atm->ShutdownATM();
	delete atm; // Delete the ATM object prior to stopping program execution.
	delete mainUI; // Delete the UI object prior to stopping program execution.

	return 0;
}
