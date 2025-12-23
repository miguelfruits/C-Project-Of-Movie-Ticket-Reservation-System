#include <fstream.h>
#include <string.h>
#include <ctype.h>

#define BOOKINGS_FILE "reservations.dat"
#define SEATS_FILE "seats.dat"

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
    
    // Search for existing record
    while (seatFile.read((char*)&seatData, sizeof(seatData))) 
	{
        if (strcmp(seatData.movieTitle, movieTitle) == 0 && 
            strcmp(seatData.showTiming, showTime) == 0) {
            recordFound = 1;
            seatFile.seekp(-(int)sizeof(seatData), ios::cur);
            strcpy(seatData.movieTitle, movieTitle);
            strcpy(seatData.showTiming, showTime);
            memcpy(seatData.seatMap, seats, sizeof(int) * 5 * 5);
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
        memcpy(seatData.seatMap, seats, sizeof(int) * 5 * 5);
        seatFile.write((char*)&seatData, sizeof(seatData));
        seatFile.close();
    }
}

void loadSeatData(char movieTitle[], char showTime[], int seats[5][5])
 {
    fstream seatFile(SEATS_FILE, ios::in | ios::binary);
    TheaterSeats seatData;
    int dataLoaded = 0;
    
    while (seatFile.read((char*)&seatData, sizeof(seatData))) 
	{
        if (strcmp(seatData.movieTitle, movieTitle) == 0 && 
            strcmp(seatData.showTiming, showTime) == 0) {
            memcpy(seats, seatData.seatMap, sizeof(int) * 5 * 5);
            dataLoaded = 1;
            break;
        }
    }
    seatFile.close();
    
    // Initialize all seats as available if no data found
    if (!dataLoaded)
		{
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
    float totalCost;

public:
    void createBookingRecord(char name[50], char movie[50], char time[20], 
                           char seats[][5], int count, float cost)
						   {
        strcpy(customerName, name);
        strcpy(movieName, movie);
        strcpy(showTime, time);
        numberOfTickets = count;
        totalCost = cost;
        for (int i = 0; i < count; i++)
            strcpy(bookedSeats[i], seats[i]);
    }

    void displayBookingDetails() {
        cout << "\n***********************************************";
        cout << "\n              BOOKING RECORD                  ";
        cout << "\n***********************************************";
        cout << "\nCustomer: " << customerName;
        cout << "\nMovie: " << movieName;
        cout << "\nShow Time: " << showTime;
        cout << "\nSeats: ";
        for (int i = 0; i < numberOfTickets; i++)
            cout << bookedSeats[i] << " ";
        cout << "\nTotal Cost: Rs. " << totalCost;
        cout << "\n***********************************************" << endl;
    }

    char* getCustomerName() { return customerName; }
};

class ShowTiming
 {
public:
    char timing[20];
    float ticketPrice;
    int seatLayout[5][5];
    char associatedMovie[50];

    ShowTiming() {}
    
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

        if (strlen(seatCode) < 2)
			{
            cout << "Invalid seat format! Please use format like A1, B2, etc." << endl;
            return 0;
        }

        // Validate row letter (A-E)
        if (seatCode[0] < 'A' || seatCode[0] > 'E')
			{
            cout << "Invalid row! Please choose between A and E." << endl;
            return 0;
        }

        // Validate seat number (1-5)
        if (seatCode[1] < '1' || seatCode[1] > '5') 
		{
            cout << "Invalid seat number! Please choose between 1 and 5." << endl;
            return 0;
        }

        int row = seatCode[0] - 'A';
        int column = seatCode[1] - '1';

        cout << "Attempting to book seat: " << seatCode << endl;

        if (seatLayout[row][column] == 0)
			{
            cout << "Sorry, seat " << seatCode << " is already booked!" << endl;
            cout << "Please choose another available seat." << endl;
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

    Movie() {}
    
    Movie(char name[], ShowTiming show1, ShowTiming show2, ShowTiming show3)
	{
        strcpy(title, name);
        shows[0] = show1;
        shows[1] = show2;
        shows[2] = show3;
        
        for (int i = 0; i < 3; i++)
			{
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

void startBookingSession(char userName[]) 
{
    clrscr();

    // Initialize movie catalog
    Movie movieCatalog[3] = 
	{
        Movie("Spider-Man: No Way Home", 
              ShowTiming("Spider-Man: No Way Home", "10:00 AM", 250), 
              ShowTiming("Spider-Man: No Way Home", "3:00 PM", 300), 
              ShowTiming("Spider-Man: No Way Home", "8:00 PM", 350)),
              
        Movie("The Avengers: Endgame", 
              ShowTiming("The Avengers: Endgame", "11:00 AM", 280), 
              ShowTiming("The Avengers: Endgame", "4:30 PM", 320), 
              ShowTiming("The Avengers: Endgame", "9:00 PM", 380)),
              
        Movie("Deadpool & Wolverine", 
              ShowTiming("Deadpool & Wolverine", "1:00 PM", 300), 
              ShowTiming("Deadpool & Wolverine", "6:30 PM", 340), 
              ShowTiming("Deadpool & Wolverine", "10:30 PM", 400))
    };

    int userChoice;
    int inBookingSession = 1;

    while (inBookingSession)
		{
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

        switch (userChoice)
		{
            case 1: {
                clrscr();
                cout << "***********************************************" << endl;
                cout << "         NOW SHOWING AT CINEMA PARADISE       " << endl;
                cout << "***********************************************" << endl << endl;
                
                for (int i = 0; i < 3; i++)
					{
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
		for (int i0 = 0; i0 < 3; i0++)
				{
		    cout << i0 + 1 << ". " << movieCatalog[i0].title << endl;
		}
		cout << "Enter your choice (1-3): ";
		cin >> selectedMovie;

		if (selectedMovie < 1 || selectedMovie > 3)
				{
		    cout << endl << "Invalid movie selection!" << endl;
		    cout << "Press any key to continue...";
		    getch();
		    break;
		}

		Movie &chosenMovie = movieCatalog[selectedMovie - 1];

		cout << endl << "Please select a show time for " << chosenMovie.title << ":" << endl;
		for (int i = 0; i < 3; i++)
				{
		    cout << i + 1 << ". " << chosenMovie.shows[i].timing;
		    cout << " - Rs." << chosenMovie.shows[i].ticketPrice << endl;
		}
		cout << "Enter your choice (1-3): ";
		cin >> selectedShow;

		if (selectedShow < 1 || selectedShow > 3)
					{
		    cout << endl << "Invalid show selection!" << endl;
		    cout << "Press any key to continue...";
		    getch();
		    break;
		}

		ShowTiming &chosenShow = chosenMovie.shows[selectedShow - 1];
		chosenShow.displaySeatChart();

		cout << endl << "How many tickets would you like to book? ";
		cin >> ticketCount;

		if (ticketCount <= 0 || ticketCount > 10)
					{
		    cout << "Invalid number! You can book 1 to 10 tickets." << endl;
		    cout << "Press any key to continue...";
		    getch();
		    break;
		}

		char selectedSeats[10][5];
		int successfulBookings = 0;

		cout << endl << "Please select your seats:" << endl;
		for (int i1 = 0; i1 < ticketCount; i1++)
					{
		    cout << "Enter seat " << (i1 + 1) << " (e.g., A1, B2): ";
		    cin >> selectedSeats[i1];

		    if (!chosenShow.reserveSeat(selectedSeats[i1]))
					{
			cout << "Please choose a different seat." << endl;
			i1--;
		    } else {
			successfulBookings++;
		    }
		}

		if (successfulBookings > 0)
					{
		    float finalAmount = successfulBookings * chosenShow.ticketPrice;

		    MovieBooking newBooking;
		    newBooking.createBookingRecord(userName, chosenMovie.title,
						 chosenShow.timing, selectedSeats,
						 successfulBookings, finalAmount);

		    fstream bookingFile(BOOKINGS_FILE, ios::app | ios::binary);
		    bookingFile.write((char*)&newBooking, sizeof(newBooking));
		    bookingFile.close();

		    cout << endl << "BOOKING CONFIRMED!" << endl;
		    cout << "***********************************************" << endl;
		    cout << "Movie: " << chosenMovie.title << endl;
		    cout << "Time: " << chosenShow.timing << endl;
		    cout << "Seats: ";
		    for (int i2 = 0; i2 < successfulBookings; i2++)
			cout << selectedSeats[i2] << " ";
		    cout << endl;
		    cout << "Total Amount: Rs." << finalAmount << endl;
		    cout << "***********************************************" << endl;
		} else
					{
		    cout << endl << "No tickets were booked." << endl;
		}

		cout << "Press any key to continue...";
		getch();
		break;
	    }

	    case 3:
			{
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
                
                while (bookingFile.read((char*)&bookingRecord, sizeof(bookingRecord)))
					{
                    if (strcmp(userName, bookingRecord.getCustomerName()) == 0) {
                        bookingRecord.displayBookingDetails();
                        foundBookings = 1;
                    }
                }
                bookingFile.close();

                if (!foundBookings)
					{
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