#include "admin.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>
#include <conio.h> // for _getch()
using namespace std;

void get_password(string &password)
{
    char pass[100]; // simple fixed-size char array
    int i = 0;
    char ch;

    // cout << "(Input hidden): ";
    while (true)
    {
        ch = _getch();

        if (ch == '\r') // Enter pressed
        {
            break;
        }
        else if (ch == '\b') // Backspace pressed
        {
            if (i > 0)
            {
                i--;
                cout << "\b \b"; // erase last '*'
            }
        }
        else if (ch >= 32 && ch <= 126) // Printable characters
        {
            pass[i++] = ch;
            cout << '*';
        }
    }
    pass[i] = '\0'; // terminate string
    cout << endl;
    password = pass;
}

void register_user()
{
    string username, password;
    cout << "\n=== Register ===\n";
    cout << "Enter new username: ";
    cin >> username;
    cout << "Enter new password: ";
    get_password(password);

    ifstream inFile("loginfile.csv");
    if (inFile.is_open())
    {
        string line;
        getline(inFile, line);
        while (getline(inFile, line))
        {
            stringstream ss(line);
            string file_un, file_pass;
            getline(ss, file_un, ',');
            getline(ss, file_pass);
            if (file_un == username)
            {
                cout << "Username already exists.\n";
                inFile.close();
                return;
            }
        }
        inFile.close();
    }

    fstream outFile("loginfile.csv", ios::in | ios::out | ios::app);
    if (!outFile.is_open())
    {
        cout << "Error opening file.\n";
        return;
    }

    ifstream checkFile("loginfile.csv");
    checkFile.seekg(0, ios::end);
    if (checkFile.tellg() == 0)
    {
        outFile << "UserName,Password\n";
    }
    else
    {
        checkFile.seekg(-1, ios::end);
        char lastChar;
        checkFile.get(lastChar);
        if (lastChar != '\n')
        {
            outFile << '\n';
        }
    }
    checkFile.close();

    outFile << username << "," << password << "\n";
    outFile.close();
    cout << "User registered successfully!\n";
}

int login_user()
{
    string input_un, input_pass;
    int attempts = 0;

    while (attempts < 3)
    {
        cout << "\n=== Login (Attempt " << attempts + 1 << " of 3) ===\n";
        cout << "Enter username: ";
        cin >> input_un;
        cout << "Enter password: ";
        get_password(input_pass);

        ifstream inFile("loginfile.csv");
        if (!inFile.is_open())
        {
            cout << "Error: Could not open file.\n";
            return 0;
        }

        string line;
        getline(inFile, line);
        bool success = false;
        while (getline(inFile, line))
        {
            stringstream ss(line);
            string file_un, file_pass;
            getline(ss, file_un, ',');
            getline(ss, file_pass);
            if (input_un == file_un && input_pass == file_pass)
            {
                success = true;
                break;
            }
        }
        inFile.close();

        if (success)
        {
            cout << "Access granted. Welcome, " << input_un << "!\n";
            return 1;
        }
        else
        {
            cout << "Access denied.\n";
            attempts++;
        }
    }
    cout << "Too many failed attempts.\n";
    return 0;
}

int admin()
{
    int option;
    cout << "\n=== Welcome to the Login System ===\n";
    do
    {
        cout << "\n1. Register\n2. Login\n3. Exit\n";
        cout << "Choose option: ";

        if (!(cin >> option))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }

        switch (option)
        {
        case 1:
            register_user();
            break;
        case 2:
            if (login_user())
                return 0;
            break;
        case 3:
            cout << "Exiting...\n";
            return 1;
        default:
            cout << "Invalid choice.\n";
        }
    } while (true);
}
