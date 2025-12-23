#include <fstream.h>
#include <string.h>

#define USER_DATA_FILE "users.dat"

class UserAccount
 {
    char userName[50];
    char userEmail[50];
    char userPassword[50];
    
public:
    void setupAccount(char name[50], char email[50], char password[50])
	{
        strcpy(userName, name);
        strcpy(userEmail, email);
        strcpy(userPassword, password);
    }
    
    char* getUserName() { return userName; }
    char* getEmail() { return userEmail; }
    char* getPassword() { return userPassword; }
};

void getPasswordInput(char password[50]) 
{
    char inputChar;
    int charPosition = 0;
    
    while (1) {
        inputChar = getch();
        
        if (inputChar == 13)
			{ 
            break;
        } else if (inputChar == 8 && charPosition > 0)
			{ 
            cout << "\b \b";
            charPosition--;
        } else if (inputChar != 8) 
		{ // Regular character
            cout << "*";
            password[charPosition++] = inputChar;
        }
    }
    password[charPosition] = '\0';
}

void createNewAccount() {
    clrscr();
    UserAccount newUser;
    char name[50], email[50], password[50];

    cout << "***********************************************" << endl;
    cout << "           CREATE NEW ACCOUNT                 " << endl;
    cout << "***********************************************" << endl << endl;
    
    cout << "Please enter your details:" << endl;
    cout << "--------------------------" << endl;
    
    cout << "Full Name: ";
    cin.ignore();
    cin.getline(name, 50);
    
    cout << "Email Address: ";
    cin.getline(email, 50);
    
    cout << "Password: ";
    getPasswordInput(password);
    
    // Save user account to file
    fstream userFile(USER_DATA_FILE, ios::app | ios::binary);
    newUser.setupAccount(name, email, password);
    userFile.write((char*)&newUser, sizeof(newUser));
    userFile.close();

    cout << endl << endl;
    cout << "Account created successfully!" << endl;
    cout << "Welcome to Cinema Paradise, " << name << "!" << endl;
    cout << endl << "Press any key to continue...";
    getch();
}

int loginUser(char userName[50])
 {
    clrscr();
    char email[50], password[50];
    
    cout << "***********************************************" << endl;
    cout << "              USER LOGIN                      " << endl;
    cout << "***********************************************" << endl << endl;
    
    cout << "Please enter your login credentials:" << endl;
    cout << "------------------------------------" << endl;
    
    cout << "Email Address: ";
    cin.ignore();
    cin.getline(email, 50);
    
    cout << "Password: ";
    getPasswordInput(password);

    // Check user credentials
    fstream userFile(USER_DATA_FILE, ios::in | ios::binary);
    if (!userFile)
		{
        cout << endl << endl;
        cout << "No user accounts found!" << endl;
        cout << "Please create an account first." << endl;
        cout << "Press any key to continue...";
        getch();
        userFile.close();
        return 0;
    }

    UserAccount existingUser;
    int loginSuccess = 0;
    
    while (userFile.read((char*)&existingUser, sizeof(existingUser))) 
	{
        if (strcmp(email, existingUser.getEmail()) == 0 && 
            strcmp(password, existingUser.getPassword()) == 0)
			{
            strcpy(userName, existingUser.getUserName());
            loginSuccess = 1;
            break;
        }
    }
    userFile.close();

    if (loginSuccess) 
	{
        cout << endl << endl;
        cout << "Login successful!" << endl;
        cout << "Welcome back, " << userName << "!" << endl;
        cout << "Press any key to continue to booking...";
        getch();
        return 1;
    } else 
	{
        cout << endl << endl;
        cout << "Login failed!" << endl;
        cout << "Invalid email or password. Please try again." << endl;
        cout << "Press any key to continue...";
        getch();
        return 0;
    }
}