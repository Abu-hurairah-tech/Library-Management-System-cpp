#include <iostream>
#include <string>
#include <limits>
#include <cctype>
#include "admin.h"
#include "member.h"
#include "books.h"
#include "issue.h"
#include "fine.h"
#include "return.h"
using namespace std;

int admin();
void manage_members();
void manage_books();
void issuing_books();
void return_books();
void manage_fine();

void displaymenu()
{
    cout << "\nWhat do you want to use:\n";
    cout << "A. Member Management System\n";
    cout << "B. Books Management System\n";
    cout << "C. Book Issue Management System\n";
    cout << "D. Book Return Management System\n";
    cout << "E. Fine Management System\n";
    cout << "F. Exit\n";
}

int main()
{
    system("cls");
    system("color 70");
    int login_check = admin();
    system("cls");
    if (login_check == 0)
    {
        char portalChoice;
        cout << "\n\n\t\t\t=== Welcome to Library Management System ===\n\n";

        do
        {
            displaymenu();
            cout << "Enter your choice: ";

            if (!(cin >> portalChoice))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Exiting.\n";
                return 1;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            portalChoice = toupper(portalChoice);

            switch (portalChoice)
            {
            case 'A':
                system("cls");
                manage_members();
                break;
            case 'B':
                system("cls");
                manage_books();
                break;
            case 'C':
                system("cls");
                issuing_books();
                break;
            case 'D':
                system("cls");
                return_books();
                break;
            case 'E':
                system("cls");
                manage_fine();
                break;
            case 'F':
                cout << "Exiting...\n\n";
                break;
            default:
                cout << "Invalid input. Please try again.\n";
            }

        } while (portalChoice != 'F');
    }
    else
    {
        cout << "Login failed. Exiting the program.\n\n";
        return 1;
    }

    return 0;
}
