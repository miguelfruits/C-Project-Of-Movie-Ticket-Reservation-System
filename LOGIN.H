#ifndef LOGIN_H
#define LOGIN_H

#include <iostream.h>
#include <conio.h>
#include <string.h>
#include <fstream.h>

#define USER_DATA_FILE "users.dat"

// Function prototypes
void createNewAccount();
int loginUser(char userName[50]);

class UserAccount
{
    char userName[50];
    char userEmail[50];
    char userPassword[50];
    
public:
    UserAccount() {
        userName[0] = '\0';
        userEmail[0] = '\0';
        userPassword[0] = '\0';
    }
    
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

void createNewAccount() {
    clrscr();
    UserAccount newUser;
    char name[50], email[50], password[50];
    char confirmPassword[50];

    cout << "***********************************************" << endl;
    cout << "           CREATE NEW ACCOUNT                 " << endl;
    cout << "***********************************************" << endl << endl;
    
    cout << "Please enter your details:" << endl;
    cout << "--------------------------" << endl;
    
    cout << "Full Name: ";
    cin.ignore();
    cin.getline(name, 50);
    
    // Validate name
    if (strlen(name) == 0) {
        cout << "\nError: Name cannot be empty!" << endl;
        cout << "Press any key to continue...";
        getch();
        return;
    }
    
    cout << "Email Address: ";
    cin.getline(email, 50);
    
    // Simple email validation
    int atFound = 0, dotFound = 0;
    for (int i = 0; email[i] != '\0'; i++) {
        if (email[i] == '@') atFound = 1;
        if (email[i] == '.' && atFound) dotFound = 1;
    }
    
    if (!atFound || !dotFound) {
        cout << "\nError: Invalid email address!" << endl;
        cout << "Press any key to continue...";
        getch();
        return;
    }
    
    cout << "Password (min 6 characters): ";
    // Simple password input without masking for Turbo C++
    cin.getline(password, 50);
    
    // Validate password length
    if (strlen(password) < 6) {
        cout << "\nError: Password must be at least 6 characters long!" << endl;
        cout << "Press any key to continue...";
        getch();
        return;
    }
    
    cout << "\nConfirm Password: ";
    cin.getline(confirmPassword, 50);
    
    // Check if passwords match
    if (strcmp(password, confirmPassword) != 0) {
        cout << "\nError: Passwords do not match!" << endl;
        cout << "Press any key to continue...";
        getch();
        return;
    }
    
    // Check if email already exists
    fstream checkFile(USER_DATA_FILE, ios::in | ios::binary);
    if (checkFile) {
        UserAccount tempUser;
        while (checkFile.read((char*)&tempUser, sizeof(tempUser))) {
            if (strcmp(email, tempUser.getEmail()) == 0) {
                cout << "\nError: Email already registered!" << endl;
                cout << "Please use a different email or login." << endl;
                checkFile.close();
                cout << "Press any key to continue...";
                getch();
                return;
            }
        }
        checkFile.close();
    }
    
    // Save user account to file
    fstream userFile(USER_DATA_FILE, ios::app | ios::binary);
    if (!userFile) {
        // If file doesn't exist, create it
        userFile.open(USER_DATA_FILE, ios::out | ios::binary);
        userFile.close();
        userFile.open(USER_DATA_FILE, ios::app | ios::binary);
    }
    
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
    cin.getline(password, 50);

    // Check user credentials
    fstream userFile(USER_DATA_FILE, ios::in | ios::binary);
    if (!userFile)
    {
        cout << endl << endl;
        cout << "Error: No user accounts found!" << endl;
        cout << "Please create an account first." << endl;
        cout << "Press any key to continue...";
        getch();
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

#endif