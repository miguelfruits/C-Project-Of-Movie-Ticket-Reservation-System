#include <iostream.h>
#include <conio.h>
#include <stdlib.h>
#include "login.h"
#include "booking.h"

void displayWelcomeScreen() 
{
    clrscr();
    cout << "***********************************************" << endl;
    cout << "       CINEMA PARADISE BOOKING SYSTEM         " << endl;
    cout << "***********************************************" << endl;
    cout << "       Your Gateway to Movie Magic           " << endl;
    cout << "***********************************************" << endl;
    cout << endl;
}

void main()
 {
    int userChoice;
    char currentUserName[50];
    int programRunning = 1;

    while (programRunning)
		{
        displayWelcomeScreen();
        
        cout << "Please choose an option:" << endl;
        cout << "1. Create New Account" << endl;
        cout << "2. Login to Existing Account" << endl;
        cout << "3. Exit Program" << endl;
        cout << "----------------------------------------" << endl;
        cout << "Enter your choice (1-3): ";
        cin >> userChoice;

        switch (userChoice) 
		{
            case 1:
                createNewAccount();
                break;
                
            case 2:
                if (loginUser(currentUserName))
					{
                    startBookingSession(currentUserName);
                }
                break;
                
            case 3:
                clrscr();
                cout << endl << endl;
                cout << "***********************************************" << endl;
                cout << "   Thank you for choosing Cinema Paradise     " << endl;
                cout << "         We hope to see you again soon        " << endl;
                cout << "***********************************************" << endl;
                cout << endl << "Press any key to exit...";
                getch();
                programRunning = 0;
                break;
                
            default:
                cout << endl << "Invalid choice! Please select 1, 2, or 3." << endl;
                cout << "Press any key to continue...";
                getch();
                break;
        }
    }
}