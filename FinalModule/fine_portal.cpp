#include "fine.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>
#include <limits>
#include <iomanip>
using namespace std;

void slip_generation()
{
    string member_id, line;
    char choice;

    do
    {
        int total_fine = 0;
        bool found_records = false;

        cout << "Enter Member ID: ";
        if (cin.peek() == '\n')
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, member_id);

        ifstream fineFile("fine.csv");
        if (!fineFile.is_open())
        {
            cout << "Unable to open fine.csv file!\n";
            return;
        }

        cout << "-------------------------------------------------------------\n";
        cout << "| " << left << setw(8) << "Book ID"
             << " | " << left << setw(10) << "Member ID"
             << " | " << left << setw(12) << "Issue Date"
             << " | " << left << setw(12) << "Return Date"
             << " | " << left << setw(6) << "Days"
             << " | " << left << setw(6) << "Fine" << " |\n";
        cout << "-------------------------------------------------------------\n";

        getline(fineFile, line); // skip header

        while (getline(fineFile, line))
        {
            stringstream ss(line);
            string book_id, user_id, doi, dor, days_late, fine_amount;

            getline(ss, book_id, ',');
            getline(ss, user_id, ',');
            getline(ss, doi, ',');
            getline(ss, dor, ',');
            getline(ss, days_late, ',');
            getline(ss, fine_amount, ',');

            if (user_id == member_id)
            {
                cout << "| " << left << setw(8) << book_id
                     << " | " << left << setw(10) << user_id
                     << " | " << left << setw(12) << doi
                     << " | " << left << setw(12) << dor
                     << " | " << left << setw(6) << days_late
                     << " | " << left << setw(6) << fine_amount << " |\n";

                if (!fine_amount.empty())
                    total_fine += stoi(fine_amount);

                found_records = true;
            }
        }

        fineFile.close();

        cout << "-------------------------------------------------------------\n";

        if (found_records)
        {
            cout << "Total Fine for Member " << member_id << " = " << total_fine << "\n";
        }
        else
        {
            cout << "No fine records found for Member ID: " << member_id << "\n";
        }

        cout << "\nGenerate another slip? (y/n): ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

    } while (choice == 'y' || choice == 'Y');
}

void clear_fine_records()
{
    string member_id, line;
    char choice;

    do
    {
        cout << "Enter Member ID to clear fine: ";
        if (cin.peek() == '\n')
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, member_id);

        ifstream fineFile("fine.csv");
        ofstream tempFile("temp.csv");

        if (!fineFile.is_open() || !tempFile.is_open())
        {
            cout << "Error opening file!\n";
            return;
        }

        bool found = false;

        if (getline(fineFile, line))
        {
            tempFile << line << "\n";
        }

        while (getline(fineFile, line))
        {
            string originalLine = line;
            stringstream ss(line);
            string book_id, user_id;

            getline(ss, book_id, ',');
            getline(ss, user_id, ',');

            if (user_id != member_id)
            {
                tempFile << originalLine << "\n";
            }
            else
            {
                found = true;
            }
        }

        fineFile.close();
        tempFile.close();

        if (found)
        {
            remove("fine.csv");
            rename("temp.csv", "fine.csv");
            cout << "Fine cleared for Member ID: " << member_id << "\n";
        }
        else
        {
            remove("temp.csv");
            cout << "No fine records found for Member ID: " << member_id << "\n";
        }

        cout << "\nClear another fine? (y/n): ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

    } while (choice == 'y' || choice == 'Y');
}

void fine()
{
    int choice;

    do
    {
        cout << "\n=== Fine Portal ===\n";
        cout << "1. Fine Slip\n";
        cout << "2. Fine Clearance\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice))
        {
            cout << "Invalid input. Try again.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1)
        {
            system("cls");
            slip_generation();
        }
        else if (choice == 2)
        {
            system("cls");
            clear_fine_records();
        }
        else if (choice == 3)
        {
            cout << "Exiting Fine Portal.\n";
            system("cls");
            break;
        }
        else
        {
            cout << "Invalid choice.\n";
        }

    } while (true);
}

void manage_fine()
{
    fine();
}
