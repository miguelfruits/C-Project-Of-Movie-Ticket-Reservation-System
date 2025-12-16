#include <iostream.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream.h>
#include <stdio.h>
#include <iomanip.h>
#include "login.h"
#include "booking.h"

#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "cinema123"

// Boolean type for Turbo C++
typedef int bool;
const int true = 1;
const int false = 0;

// Function prototypes
void displayWelcomeScreen();
void displayAdminMenu();
void adminFunctions();
int adminLogin();
void viewAbout();

void displayWelcomeScreen() 
{
    clrscr();
    cout << "***********************************************" << endl;
    cout << "       CINEMA PARADISE BOOKING SYSTEM         " << endl;
    cout << "***********************************************" << endl;
    cout << "       Your Gateway to Movie Magic           " << endl;
    cout << "***********************************************" << endl;
    cout << "         With Food & Beverage Service        " << endl;
    cout << "***********************************************" << endl;
    cout << endl;
}

void displayAdminMenu() 
{
    clrscr();
    cout << "***********************************************" << endl;
    cout << "           ADMINISTRATION PANEL               " << endl;
    cout << "***********************************************" << endl;
    cout << "1. View All Bookings" << endl;
    cout << "2. View Revenue Report" << endl;
    cout << "3. Reset Seat Data" << endl;
    cout << "4. Back to Main Menu" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Enter your choice: ";
}

void adminFunctions() 
{
    int adminChoice;
    bool inAdminMenu = true;
    
    while (inAdminMenu) 
    {
        displayAdminMenu();
        cin >> adminChoice;
        
        switch (adminChoice) 
        {
            case 1: {
                // View all bookings
                clrscr();
                cout << "***********************************************" << endl;
                cout << "           ALL BOOKING RECORDS                " << endl;
                cout << "***********************************************" << endl << endl;
                
                fstream bookingFile(BOOKINGS_FILE, ios::in | ios::binary);
                if (!bookingFile) 
                {
                    cout << "No bookings found in the system." << endl;
                } 
                else 
                {
                    MovieBooking bookingRecord;
		    int totalBookings = 0;
		    int left,right;
                    float totalRevenue = 0;
                    
                    cout << "=================================================================" << endl;
                    cout << "SR CUSTOMER NAME     MOVIE               TIME    TICKETS TOTAL" << endl;
                    cout << "=================================================================" << endl;
                    
                    int serialNo = 1;
                    while (bookingFile.read((char*)&bookingRecord, sizeof(bookingRecord))) 
                    {
                        // Serial Number
                        cout << setw(2) << serialNo++ << " ";
                        
                        // Customer Name
                        cout << setw(16) << left << bookingRecord.getCustomerName() << " ";
                        
                        // Movie Name
                        cout << setw(18) << left << bookingRecord.getMovieName() << " ";
                        
                        // Show Time
                        cout << setw(8) << left << bookingRecord.getShowTime() << " ";
                        
                        // Number of Tickets
                        cout << setw(7) << right << bookingRecord.getNumberOfTickets() << " ";
                        
                        // Total Cost
                        cout << "Rs." << setw(8) << left << bookingRecord.getTotalCost();
                        
                        cout << endl;
                        
                        totalBookings++;
                        totalRevenue += bookingRecord.getTotalCost();
                    }
                    
                    cout << "=================================================================" << endl;
                    cout << "\nDETAILED VIEW:" << endl;
                    cout << "-----------------------------------------------" << endl;
                    
                    // Show detailed view
                    bookingFile.clear();
                    bookingFile.seekg(0, ios::beg);
                    serialNo = 1;
                    
                    while (bookingFile.read((char*)&bookingRecord, sizeof(bookingRecord))) 
                    {
                        cout << "\nBooking #" << serialNo++ << ":" << endl;
                        bookingRecord.displayAdminBookingDetails();
                        cout << endl;
                    }
                    
                    cout << "\nSUMMARY:" << endl;
                    cout << "Total Bookings: " << totalBookings << endl;
                    cout << "Total Revenue: Rs." << totalRevenue << endl;
                    cout << "***********************************************" << endl;
                    
                    bookingFile.close();
                }
                
                cout << "\nPress any key to continue...";
                getch();
                break;
            }
            
            case 2: {
                // View revenue report
                clrscr();
                cout << "***********************************************" << endl;
                cout << "           REVENUE REPORT                     " << endl;
                cout << "***********************************************" << endl << endl;
                
                fstream bookingFile(BOOKINGS_FILE, ios::in | ios::binary);
                if (!bookingFile) 
                {
                    cout << "No revenue data available." << endl;
                } 
                else 
                {
                    MovieBooking bookingRecord;
                    float totalTicketRevenue = 0;
                    float totalFoodRevenue = 0;
                    float grandTotal = 0;
                    int bookingCount = 0;
                    
                    while (bookingFile.read((char*)&bookingRecord, sizeof(bookingRecord))) 
                    {
                        totalTicketRevenue += bookingRecord.getTicketCost();
                        totalFoodRevenue += bookingRecord.getFoodCost();
                        grandTotal += bookingRecord.getTotalCost();
                        bookingCount++;
                    }
                    
                    cout << "Total Bookings: " << bookingCount << endl;
                    cout << "-----------------------------------------------" << endl;
                    cout << "Ticket Revenue: Rs." << totalTicketRevenue << endl;
                    cout << "Food & Beverage Revenue: Rs." << totalFoodRevenue << endl;
                    cout << "-----------------------------------------------" << endl;
                    cout << "Grand Total Revenue: Rs." << grandTotal << endl;
                    cout << "***********************************************" << endl;
                    
                    bookingFile.close();
                }
                
                cout << "\nPress any key to continue...";
                getch();
                break;
            }
            
            case 3: {
                // Reset seat data
                clrscr();
                cout << "***********************************************" << endl;
                cout << "           RESET SEAT DATA                    " << endl;
                cout << "***********************************************" << endl << endl;
                
                char confirm;
                cout << "WARNING: This will reset ALL seat data for all movies and shows!" << endl;
                cout << "All seat bookings will be lost. Are you sure? (Y/N): ";
                cin >> confirm;
                
                if (toupper(confirm) == 'Y' || toupper(confirm) == 'y') 
                {
                    // Delete seat data file
                    int result = remove(SEATS_FILE);
                    if (result == 0) {
                        cout << "\nSeat data has been reset successfully!" << endl;
                        cout << "All seats are now available for booking." << endl;
                    } else {
                        cout << "\nError: Could not reset seat data." << endl;
                        cout << "File may not exist or is in use." << endl;
                    }
                } 
                else 
                {
                    cout << "\nOperation cancelled." << endl;
                }
                
                cout << "\nPress any key to continue...";
                getch();
                break;
            }
            
            case 4:
                inAdminMenu = false;
                break;
                
            default:
                cout << "\nInvalid choice! Please select 1-4." << endl;
                cout << "Press any key to continue...";
                getch();
                break;
        }
    }
}

int adminLogin() 
{
    char username[50];
    char password[50];
    
    clrscr();
    cout << "***********************************************" << endl;
    cout << "              ADMIN LOGIN                     " << endl;
    cout << "***********************************************" << endl;
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    
    // Simple password masking
    int i = 0;
    char ch;
    while (1)
    {
        ch = getch();
        if (ch == 13) // Enter key
            break;
        else if (ch == 8) // Backspace
        {
            if (i > 0) 
            {
                cout << "\b \b";
                i--;
            }
        }
        else 
        {
            password[i] = ch;
            cout << "*";
            i++;
        }
    }
    password[i] = '\0';
    
    if (strcmp(username, ADMIN_USERNAME) == 0 && 
        strcmp(password, ADMIN_PASSWORD) == 0) 
    {
        cout << "\n\nLogin successful!" << endl;
        cout << "Press any key to continue...";
        getch();
        return 1;
    } 
    else 
    {
        cout << "\n\nInvalid username or password!" << endl;
        cout << "Press any key to continue...";
        getch();
        return 0;
    }
}

void viewAbout() 
{
    clrscr();
    cout << "***********************************************" << endl;
    cout << "        ABOUT CINEMA PARADISE SYSTEM          " << endl;
    cout << "***********************************************" << endl;
    cout << endl;
    cout << "Cinema Paradise Booking System v2.0" << endl;
    cout << "Developed for: Movie Theater Management" << endl;
    cout << endl;
    cout << "FEATURES:" << endl;
    cout << "1. User Registration & Login" << endl;
    cout << "2. Movie Ticket Booking" << endl;
    cout << "3. Seat Selection (5x5 Layout)" << endl;
    cout << "4. Food & Beverage Ordering" << endl;
    cout << "5. Booking History" << endl;
    cout << "6. Admin Panel" << endl;
    cout << endl;
    cout << "FOOD MENU:" << endl;
    cout << "- Small Popcorn: Rs.150" << endl;
    cout << "- Medium Popcorn: Rs.250" << endl;
    cout << "- Large Popcorn: Rs.350" << endl;
    cout << "- Cold Drink: Rs.80" << endl;
    cout << "- Nachos: Rs.200" << endl;
    cout << "- Combo Meal: Rs.500" << endl;
    cout << endl;
    cout << "***********************************************" << endl;
    cout << "\nPress any key to continue...";
    getch();
}

int main()
{
    int userChoice;
    char currentUserName[50];
    bool programRunning = true;

    while(programRunning)
    {
        displayWelcomeScreen();

        cout << "Please choose an option:" << endl;
        cout << "1. Create New Account" << endl;
        cout << "2. Login to Existing Account" << endl;
        cout << "3. Admin Login" << endl;
        cout << "4. About This System" << endl;
        cout << "5. Exit Program" << endl;
        cout << "----------------------------------------" << endl;
        cout << "Enter your choice (1-5): ";
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
                if(adminLogin())
                {
                    adminFunctions();
                }
                break;

            case 4:
                viewAbout();
                break;

            case 5:
                clrscr();
                cout << endl << endl;
                cout << "***********************************************" << endl;
                cout << "   Thank you for choosing Cinema Paradise     " << endl;
                cout << "         We hope to see you again soon        " << endl;
                cout << "***********************************************" << endl;
                cout << endl << "Press any key to exit...";
                getch();
                programRunning = false;
                break;

            default:
                cout << endl << "Invalid choice! Please select 1-5." << endl;
                cout << "Press any key to continue...";
                getch();
                break;
        }
    }
    return 0;
}