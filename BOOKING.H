#ifndef BOOKING_H
#define BOOKING_H

#include <fstream.h>
#include <iostream.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>
#include <iomanip.h>

#define BOOKINGS_FILE "reservations.dat"
#define SEATS_FILE "seats.dat"

// Function prototypes
void startBookingSession(char userName[]);
float getFoodOrder(int &popcornSmall, int &popcornMedium, int &popcornLarge,
                  int &coldDrink, int &nachos, int &comboMeal);

// Structure to store seat information
struct TheaterSeats 
{
    char movieTitle[50];
    char showTiming[20];
    int seatMap[5][5];
};

// Seat management functions
void saveSeatData(char movieTitle[], char showTime[], int seats[5][5])
{
    fstream seatFile(SEATS_FILE, ios::in | ios::out | ios::binary);
    TheaterSeats seatData;
    int recordFound = 0;
    
    if (!seatFile) {
        // If file doesn't exist, create it
        seatFile.open(SEATS_FILE, ios::out | ios::binary);
        seatFile.close();
        seatFile.open(SEATS_FILE, ios::in | ios::out | ios::binary);
    }
    
    // Search for existing record
    while (seatFile.read((char*)&seatData, sizeof(seatData))) 
    {
        if (strcmp(seatData.movieTitle, movieTitle) == 0 && 
            strcmp(seatData.showTiming, showTime) == 0) {
            recordFound = 1;
            seatFile.seekp(-(int)sizeof(seatData), ios::cur);
            strcpy(seatData.movieTitle, movieTitle);
            strcpy(seatData.showTiming, showTime);
            
            // Copy seat data
            for(int i = 0; i < 5; i++)
                for(int j = 0; j < 5; j++)
                    seatData.seatMap[i][j] = seats[i][j];
                    
            seatFile.write((char*)&seatData, sizeof(seatData));
            break;
        }
    }
    seatFile.close();
    
    // Create new record if not found
    if (!recordFound) {
        seatFile.open(SEATS_FILE, ios::app | ios::binary);
        strcpy(seatData.movieTitle, movieTitle);
        strcpy(seatData.showTiming, showTime);
        
        // Copy seat data
        for(int i = 0; i < 5; i++)
            for(int j = 0; j < 5; j++)
                seatData.seatMap[i][j] = seats[i][j];
                
        seatFile.write((char*)&seatData, sizeof(seatData));
        seatFile.close();
    }
}

void loadSeatData(char movieTitle[], char showTime[], int seats[5][5])
{
    fstream seatFile(SEATS_FILE, ios::in | ios::binary);
    TheaterSeats seatData;
    int dataLoaded = 0;
    
    if (!seatFile) {
        // Initialize all seats as available if file doesn't exist
        for (int row = 0; row < 5; row++)
            for (int col = 0; col < 5; col++)
                seats[row][col] = 1;
        return;
    }
    
    while (seatFile.read((char*)&seatData, sizeof(seatData))) 
    {
        if (strcmp(seatData.movieTitle, movieTitle) == 0 && 
            strcmp(seatData.showTiming, showTime) == 0) {
            
            // Copy seat data
            for(int i = 0; i < 5; i++)
                for(int j = 0; j < 5; j++)
                    seats[i][j] = seatData.seatMap[i][j];
                    
            dataLoaded = 1;
            break;
        }
    }
    seatFile.close();
    
    // Initialize all seats as available if no data found
    if (!dataLoaded) {
        for (int row = 0; row < 5; row++)
            for (int col = 0; col < 5; col++)
                seats[row][col] = 1;
    }
}

class MovieBooking
{
    char customerName[50];
    char movieName[50];
    char showTime[20];
    char bookedSeats[10][5];
    int numberOfTickets;
    float ticketCost;
    float foodCost;
    float totalCost;
    int popcornSmall;
    int popcornMedium;
    int popcornLarge;
    int coldDrink;
    int nachos;
    int comboMeal;

public:
    MovieBooking() {
        // Initialize all values
        customerName[0] = '\0';
        movieName[0] = '\0';
        showTime[0] = '\0';
        numberOfTickets = 0;
        ticketCost = 0.0;
        foodCost = 0.0;
        totalCost = 0.0;
        popcornSmall = popcornMedium = popcornLarge = 0;
        coldDrink = nachos = comboMeal = 0;
        for(int i = 0; i < 10; i++)
            bookedSeats[i][0] = '\0';
    }
    
    void createBookingRecord(char name[50], char movie[50], char time[20], 
                           char seats[][5], int count, float tCost, float fCost,
                           int pSmall, int pMedium, int pLarge,
                           int drink, int nach, int combo) {
        strcpy(customerName, name);
        strcpy(movieName, movie);
        strcpy(showTime, time);
        numberOfTickets = count;
        ticketCost = tCost;
        foodCost = fCost;
        totalCost = tCost + fCost;
        popcornSmall = pSmall;
        popcornMedium = pMedium;
        popcornLarge = pLarge;
        coldDrink = drink;
        nachos = nach;
        comboMeal = combo;
        
        for (int i = 0; i < count; i++) {
            if (i < 10)  // Safety check
                strcpy(bookedSeats[i], seats[i]);
        }
    }

    void displayBookingDetails() {
        cout << "\n***********************************************";
        cout << "\n              BOOKING RECORD                  ";
        cout << "\n***********************************************";
        cout << "\nCustomer: " << customerName;
        cout << "\nMovie: " << movieName;
        cout << "\nShow Time: " << showTime;
        cout << "\nTickets: " << numberOfTickets;
        cout << "\nSeats: ";
        for (int i = 0; i < numberOfTickets && i < 10; i++) {
            if (bookedSeats[i][0] != '\0')
                cout << bookedSeats[i] << " ";
        }
        if (numberOfTickets > 0)
            cout << "\nTicket Cost: " << numberOfTickets << " x Rs." << ticketCost/numberOfTickets;
        cout << " = Rs." << ticketCost;
        
        if (foodCost > 0) {
            cout << "\n\nFood & Beverages:";
            if (popcornSmall > 0) cout << "\n  Small Popcorn: " << popcornSmall << " x Rs.150 = Rs." << popcornSmall * 150;
            if (popcornMedium > 0) cout << "\n  Medium Popcorn: " << popcornMedium << " x Rs.250 = Rs." << popcornMedium * 250;
            if (popcornLarge > 0) cout << "\n  Large Popcorn: " << popcornLarge << " x Rs.350 = Rs." << popcornLarge * 350;
            if (coldDrink > 0) cout << "\n  Cold Drink: " << coldDrink << " x Rs.80 = Rs." << coldDrink * 80;
            if (nachos > 0) cout << "\n  Nachos: " << nachos << " x Rs.200 = Rs." << nachos * 200;
            if (comboMeal > 0) cout << "\n  Combo Meal: " << comboMeal << " x Rs.500 = Rs." << comboMeal * 500;
            cout << "\n  Total Food Cost: Rs." << foodCost;
        } else {
            cout << "\nFood & Beverages: None";
        }
        
        cout << "\n-----------------------------------------------";
        cout << "\nTotal Cost: Rs." << totalCost;
        cout << "\n***********************************************" << endl;
    }

    void displayAdminBookingDetails() {
        cout << "\nCustomer: " << customerName;
        cout << "\nMovie: " << movieName;
        cout << "\nShow Time: " << showTime;
        cout << "\nTickets: " << numberOfTickets;
        cout << "\nSeats: ";
        for (int i = 0; i < numberOfTickets && i < 10; i++) {
            if (bookedSeats[i][0] != '\0')
                cout << bookedSeats[i] << " ";
        }
        
        cout << "\nFood Order: ";
        int totalFoodItems = 0;
        if (popcornSmall > 0) { 
            cout << "Small Popcorn(" << popcornSmall << ") "; 
            totalFoodItems++;
        }
        if (popcornMedium > 0) { 
            cout << "Medium Popcorn(" << popcornMedium << ") "; 
            totalFoodItems++;
        }
        if (popcornLarge > 0) { 
            cout << "Large Popcorn(" << popcornLarge << ") "; 
            totalFoodItems++;
        }
        if (coldDrink > 0) { 
            cout << "Cold Drink(" << coldDrink << ") "; 
            totalFoodItems++;
        }
        if (nachos > 0) { 
            cout << "Nachos(" << nachos << ") "; 
            totalFoodItems++;
        }
        if (comboMeal > 0) { 
            cout << "Combo Meal(" << comboMeal << ") "; 
            totalFoodItems++;
        }
        if (totalFoodItems == 0) {
            cout << "None";
        }
        
        cout << "\nTicket Cost: Rs." << ticketCost;
        cout << "\nFood Cost: Rs." << foodCost;
        cout << "\nTotal Cost: Rs." << totalCost;
    }

    // Getter methods
    char* getMovieName() { return movieName; }
    char* getShowTime() { return showTime; }
    char* getCustomerName() { return customerName; }
    int getNumberOfTickets() { return numberOfTickets; }
    float getTicketCost() { return ticketCost; }
    float getFoodCost() { return foodCost; }
    float getTotalCost() { return totalCost; }
    
    int getPopcornSmall() { return popcornSmall; }
    int getPopcornMedium() { return popcornMedium; }
    int getPopcornLarge() { return popcornLarge; }
    int getColdDrink() { return coldDrink; }
    int getNachos() { return nachos; }
    int getComboMeal() { return comboMeal; }
};

class ShowTiming
{
public:
    char timing[20];
    float ticketPrice;
    int seatLayout[5][5];
    char associatedMovie[50];

    ShowTiming() {
        timing[0] = '\0';
        ticketPrice = 0.0;
        associatedMovie[0] = '\0';
        // Initialize seat layout to available (1)
        for(int i = 0; i < 5; i++)
            for(int j = 0; j < 5; j++)
                seatLayout[i][j] = 1;
    }
    
    ShowTiming(char movie[], char time[], float price) {
        strcpy(associatedMovie, movie);
        strcpy(timing, time);
        ticketPrice = price;
        loadSeatData(associatedMovie, timing, seatLayout);
    }

    void displaySeatChart() 
    {
        char rowLetter = 'A';
        cout << "\nCURRENT SEAT AVAILABILITY";
        cout << "\n   ";
        for (int seatNum = 1; seatNum <= 5; seatNum++)
            cout << seatNum << " ";
        cout << "\n";
        cout << "  +-----------------+";

        for (int row = 0; row < 5; row++) {
            cout << "\n" << rowLetter++ << " | ";
            for (int col = 0; col < 5; col++) {
                if (seatLayout[row][col] == 1) {
                    cout << "1 "; // Available seat
                } else {
                    cout << "0 "; // Booked seat
                }
            }
            cout << "|";
        }
        cout << "\n  +-----------------+";
        cout << "\n1 = Available  0 = Booked";
        cout << "\n*******************************" << endl;
    }

    int reserveSeat(char seatCode[5])
    {
        // Convert to uppercase for consistency
        for (int i = 0; seatCode[i]; i++) 
        {
            seatCode[i] = toupper(seatCode[i]);
        }

        if (strlen(seatCode) < 2) {
            cout << "Invalid seat format! Please use format like A1, B2, etc." << endl;
            return 0;
        }

        // Validate row letter (A-E)
        if (seatCode[0] < 'A' || seatCode[0] > 'E') {
            cout << "Invalid row! Please choose between A and E." << endl;
            return 0;
        }

        // Validate seat number (1-5)
        int seatNum = seatCode[1] - '0';
        if (seatNum < 1 || seatNum > 5) {
            cout << "Invalid seat number! Please choose between 1 and 5." << endl;
            return 0;
        }

        int row = seatCode[0] - 'A';
        int column = seatNum - 1;

        if (seatLayout[row][column] == 0) {
            cout << "Sorry, seat " << seatCode << " is already booked!" << endl;
            return 0;
        }

        seatLayout[row][column] = 0;
        saveSeatData(associatedMovie, timing, seatLayout);
        cout << "Seat " << seatCode << " reserved successfully!" << endl;
        return 1;
    }
};

class Movie 
{
public:
    char title[50];
    ShowTiming shows[3];

    Movie() {
        title[0] = '\0';
    }
    
    Movie(char name[], ShowTiming show1, ShowTiming show2, ShowTiming show3)
    {
        strcpy(title, name);
        shows[0] = show1;
        shows[1] = show2;
        shows[2] = show3;
        
        for (int i = 0; i < 3; i++) {
            strcpy(shows[i].associatedMovie, title);
        }
    }

    void displayMovieInfo(int index)
    {
        cout << index + 1 << ". " << title << endl;
        for (int i = 0; i < 3; i++) 
        {
            cout << "   " << i + 1 << ". " << shows[i].timing;
            cout << " | Rs." << shows[i].ticketPrice << endl;
        }
    }
};

// Function to display food menu and get selections
float getFoodOrder(int &popcornSmall, int &popcornMedium, int &popcornLarge,
                  int &coldDrink, int &nachos, int &comboMeal) {
    char addFood;
    float totalFoodCost = 0;
    
    popcornSmall = popcornMedium = popcornLarge = 0;
    coldDrink = nachos = comboMeal = 0;
    
    cout << "\n***********************************************" << endl;
    cout << "           ADD FOOD & BEVERAGES               " << endl;
    cout << "***********************************************" << endl;
    cout << "Would you like to add food and beverages to your order? (Y/N): ";
    cin >> addFood;
    
    if (toupper(addFood) == 'Y' || toupper(addFood) == 'y') {
        int foodChoice;
        int orderingFood = 1;
        
        while (orderingFood) {
            clrscr();
            cout << "***********************************************" << endl;
            cout << "           FOOD & BEVERAGES MENU              " << endl;
            cout << "***********************************************" << endl;
            cout << "1. Small Popcorn     - Rs.150" << endl;
            cout << "2. Medium Popcorn    - Rs.250" << endl;
            cout << "3. Large Popcorn     - Rs.350" << endl;
            cout << "4. Cold Drink        - Rs.80" << endl;
            cout << "5. Nachos            - Rs.200" << endl;
            cout << "6. Combo Meal (Popcorn + Drink + Nachos) - Rs.500" << endl;
            cout << "7. Done with Food Order" << endl;
            cout << "-----------------------------------------------" << endl;
            
            // Display current food order
            cout << "\nCurrent Order:" << endl;
            if (popcornSmall > 0) cout << "  Small Popcorn: " << popcornSmall << endl;
            if (popcornMedium > 0) cout << "  Medium Popcorn: " << popcornMedium << endl;
            if (popcornLarge > 0) cout << "  Large Popcorn: " << popcornLarge << endl;
            if (coldDrink > 0) cout << "  Cold Drink: " << coldDrink << endl;
            if (nachos > 0) cout << "  Nachos: " << nachos << endl;
            if (comboMeal > 0) cout << "  Combo Meal: " << comboMeal << endl;
            cout << "  Total Food Cost: Rs." << totalFoodCost << endl;
            cout << "***********************************************" << endl;
            
            cout << "Enter your choice (1-7): ";
            cin >> foodChoice;
            
            int quantity;
            switch(foodChoice) {
                case 1:
                    cout << "How many Small Popcorn? ";
                    cin >> quantity;
                    popcornSmall += quantity;
                    totalFoodCost += quantity * 150;
                    break;
                case 2:
                    cout << "How many Medium Popcorn? ";
                    cin >> quantity;
                    popcornMedium += quantity;
                    totalFoodCost += quantity * 250;
                    break;
                case 3:
                    cout << "How many Large Popcorn? ";
                    cin >> quantity;
                    popcornLarge += quantity;
                    totalFoodCost += quantity * 350;
                    break;
                case 4:
                    cout << "How many Cold Drinks? ";
                    cin >> quantity;
                    coldDrink += quantity;
                    totalFoodCost += quantity * 80;
                    break;
                case 5:
                    cout << "How many Nachos? ";
                    cin >> quantity;
                    nachos += quantity;
                    totalFoodCost += quantity * 200;
                    break;
                case 6:
                    cout << "How many Combo Meals? ";
                    cin >> quantity;
                    comboMeal += quantity;
                    totalFoodCost += quantity * 500;
                    break;
                case 7:
                    orderingFood = 0;
                    break;
                default:
                    cout << "Invalid choice! Please select 1-7." << endl;
                    cout << "Press any key to continue...";
                    getch();
            }
        }
    }
    
    return totalFoodCost;
}

void startBookingSession(char userName[]) 
{
    clrscr();

    // Initialize movie catalog
    Movie movieCatalog[3];
    
    // Initialize with constructor calls
    ShowTiming show1("Spider-Man: No Way Home", "10:00 AM", 250);
    ShowTiming show2("Spider-Man: No Way Home", "3:00 PM", 300);
    ShowTiming show3("Spider-Man: No Way Home", "8:00 PM", 350);
    movieCatalog[0] = Movie("Spider-Man: No Way Home", show1, show2, show3);
    
    show1 = ShowTiming("The Avengers: Endgame", "11:00 AM", 280);
    show2 = ShowTiming("The Avengers: Endgame", "4:30 PM", 320);
    show3 = ShowTiming("The Avengers: Endgame", "9:00 PM", 380);
    movieCatalog[1] = Movie("The Avengers: Endgame", show1, show2, show3);
    
    show1 = ShowTiming("Deadpool & Wolverine", "1:00 PM", 300);
    show2 = ShowTiming("Deadpool & Wolverine", "6:30 PM", 340);
    show3 = ShowTiming("Deadpool & Wolverine", "10:30 PM", 400);
    movieCatalog[2] = Movie("Deadpool & Wolverine", show1, show2, show3);

    int userChoice;
    int inBookingSession = 1;

    while (inBookingSession) {
        clrscr();
        cout << "***********************************************" << endl;
        cout << "          MOVIE TICKET BOOKING                " << endl;
        cout << "***********************************************" << endl;
        cout << "Welcome, " << userName << "!" << endl;
        cout << "***********************************************" << endl;
        cout << "1. Browse Current Movies" << endl;
        cout << "2. Book Movie Tickets" << endl;
        cout << "3. View My Bookings" << endl;
        cout << "4. Return to Main Menu" << endl;
        cout << "----------------------------------------" << endl;
        cout << "Please enter your choice: ";
        cin >> userChoice;

        switch (userChoice) {
            case 1: {
                clrscr();
                cout << "***********************************************" << endl;
                cout << "         NOW SHOWING AT CINEMA PARADISE       " << endl;
                cout << "***********************************************" << endl << endl;
                
                for (int i = 0; i < 3; i++) {
                    movieCatalog[i].displayMovieInfo(i);
                    cout << endl;
                }
                cout << "Press any key to continue...";
                getch();
                break;
            }
                
            case 2: {
                clrscr();
                int selectedMovie, selectedShow, ticketCount;

                cout << "***********************************************" << endl;
                cout << "           BOOK MOVIE TICKETS                 " << endl;
                cout << "***********************************************" << endl << endl;
                
                cout << "Please select a movie:" << endl;
                for (int i = 0; i < 3; i++) {
                    cout << i + 1 << ". " << movieCatalog[i].title << endl;
                }
                cout << "Enter your choice (1-3): ";
                cin >> selectedMovie;

                if (selectedMovie < 1 || selectedMovie > 3) {
                    cout << endl << "Invalid movie selection!" << endl;
                    cout << "Press any key to continue...";
                    getch();
                    break;
                }

                Movie &chosenMovie = movieCatalog[selectedMovie - 1];

                cout << endl << "Please select a show time for " << chosenMovie.title << ":" << endl;
		for (int i1 = 0; i1 < 3; i1++) {
		    cout << i1 + 1 << ". " << chosenMovie.shows[i1].timing;
		    cout << " - Rs." << chosenMovie.shows[i1].ticketPrice << endl;
		}
		cout << "Enter your choice (1-3): ";
		cin >> selectedShow;

		if (selectedShow < 1 || selectedShow > 3) {
		    cout << endl << "Invalid show selection!" << endl;
		    cout << "Press any key to continue...";
		    getch();
		    break;
		}

		ShowTiming &chosenShow = chosenMovie.shows[selectedShow - 1];
		chosenShow.displaySeatChart();

		cout << endl << "How many tickets would you like to book? ";
		cin >> ticketCount;

		if (ticketCount <= 0 || ticketCount > 10) {
		    cout << "Invalid number! You can book 1 to 10 tickets." << endl;
		    cout << "Press any key to continue...";
		    getch();
		    break;
		}

		char selectedSeats[10][5];
		int successfulBookings = 0;

		cout << endl << "Please select your seats:" << endl;
		for (int i2 = 0; i2 < ticketCount; i2++) {
		    cout << "Enter seat " << (i2 + 1) << " (e.g., A1, B2): ";
		    cin >> selectedSeats[i2];

		    if (!chosenShow.reserveSeat(selectedSeats[i2])) {
			cout << "Please choose a different seat." << endl;
			i--;  // Decrement to retry this seat
		    } else {
			successfulBookings++;
		    }
                }

                if (successfulBookings > 0) {
                    float ticketAmount = successfulBookings * chosenShow.ticketPrice;
                    
                    // Get food order
                    int popcornSmall, popcornMedium, popcornLarge;
                    int coldDrink, nachos, comboMeal;
                    float foodAmount = getFoodOrder(popcornSmall, popcornMedium, popcornLarge,
                                                   coldDrink, nachos, comboMeal);
                    
                    float finalAmount = ticketAmount + foodAmount;

                    MovieBooking newBooking;
                    newBooking.createBookingRecord(userName, chosenMovie.title,
                                 chosenShow.timing, selectedSeats,
                                 successfulBookings, ticketAmount, foodAmount,
                                 popcornSmall, popcornMedium, popcornLarge,
                                 coldDrink, nachos, comboMeal);

                    fstream bookingFile(BOOKINGS_FILE, ios::app | ios::binary);
                    if (!bookingFile) {
                        // Create file if it doesn't exist
                        bookingFile.open(BOOKINGS_FILE, ios::out | ios::binary);
                        bookingFile.close();
                        bookingFile.open(BOOKINGS_FILE, ios::app | ios::binary);
                    }
                    bookingFile.write((char*)&newBooking, sizeof(newBooking));
                    bookingFile.close();

                    cout << endl << "BOOKING CONFIRMED!" << endl;
                    cout << "***********************************************" << endl;
                    cout << "Movie: " << chosenMovie.title << endl;
                    cout << "Time: " << chosenShow.timing << endl;
                    cout << "Seats: ";
                    for (int i = 0; i < successfulBookings; i++)
                        cout << selectedSeats[i] << " ";
                    cout << endl;
                    cout << "Tickets: " << successfulBookings << " x Rs." << chosenShow.ticketPrice;
                    cout << " = Rs." << ticketAmount << endl;
                    
                    if (foodAmount > 0) {
                        cout << "Food & Beverages: Rs." << foodAmount << endl;
                    }
                    
                    cout << "Total Amount: Rs." << finalAmount << endl;
                    cout << "***********************************************" << endl;
                } else {
                    cout << endl << "No tickets were booked." << endl;
                }

                cout << "Press any key to continue...";
                getch();
                break;
            }

            case 3: {
                clrscr();
                cout << "***********************************************" << endl;
                cout << "           YOUR BOOKING HISTORY               " << endl;
                cout << "***********************************************" << endl << endl;
                
                fstream bookingFile(BOOKINGS_FILE, ios::in | ios::binary);
                if (!bookingFile) {
                    cout << "No bookings found in your account." << endl;
                    cout << "Press any key to continue...";
                    getch();
                    break;
                }

                MovieBooking bookingRecord;
                int foundBookings = 0;
                
                while (bookingFile.read((char*)&bookingRecord, sizeof(bookingRecord))) {
                    if (strcmp(userName, bookingRecord.getCustomerName()) == 0) {
                        bookingRecord.displayBookingDetails();
                        foundBookings = 1;
                    }
                }
                bookingFile.close();

                if (!foundBookings) {
                    cout << "You haven't made any bookings yet." << endl;
                }
                cout << "Press any key to continue...";
                getch();
                break;
            }
                
            case 4:
                inBookingSession = 0;
                break;
                
            default:
                cout << endl << "Invalid choice! Please select 1-4." << endl;
                cout << "Press any key to continue...";
                getch();
                break;
        }
    }
}

#endif