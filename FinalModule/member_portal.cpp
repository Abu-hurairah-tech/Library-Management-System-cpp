#include "member.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <limits>
#include <cctype>
using namespace std;

struct Member
{
    string name;
    string member_ID;
    string department;
    string session;
    string contact;
};

Member *members = nullptr;
int total_members = 0;
int max_members = 10;

static void resizeArray()
{
    max_members *= 2;
    Member *newArray = new Member[max_members];

    // Copy old data
    for (int i = 0; i < total_members; i++)
    {
        newArray[i] = members[i];
    }

    delete[] members;
    members = newArray;
}

void displayMenu()
{
    cout << "\n\n\t\t\t=== MEMBER PORTAL  ===\n\n";
    cout << "What do you want to do: \n";
    cout << "1. Add a member. \n";
    cout << "2. Search a member. \n";
    cout << "3. Delete a member. \n";
    cout << "4. Display all members.\n";
    cout << "5. Exit. \n";
}

void loadMembersFromFile()
{
    ifstream file("member.csv");
    if (!file)
    {
        cout << "No existing file found. A new one will be created.\n";
        return;
    }

    string line;
    getline(file, line);
    total_members = 0;

    while (getline(file, line))
    {
        if (!line.empty() && total_members >= max_members)
        {
            resizeArray();
        }

        stringstream ss(line);
        getline(ss, members[total_members].name, ',');
        getline(ss, members[total_members].member_ID, ',');
        getline(ss, members[total_members].department, ',');
        getline(ss, members[total_members].session, ',');
        getline(ss, members[total_members].contact);

        total_members++;
    }
    file.close();
}

void addMember()
{
    fstream file("member.csv", ios::in | ios::out | ios::app);
    if (!file)
    {
        cout << "Could not open file!\n";
        return;
    }

    // Write header if file is empty
    file.seekp(0, ios::end);
    if (file.tellp() == 0)
    {
        file << "Name,ID,Department,Session,Contact\n";
    }
    else
    {
        // Ensure last line ends with a newline
        file.seekg(-1, ios::end);
        char lastChar;
        file.get(lastChar);
        if (lastChar != '\n')
        {
            file << '\n';
        }
    }

    char choice;
    do
    {
        if (total_members >= max_members)
        {
            resizeArray();
        }

        bool validInput = false;
        while (!validInput)
        {
            // Validate Name - only characters and spaces
            bool validName = false;
            while (!validName)
            {
                cout << "1. Name: ";
                getline(cin, members[total_members].name);

                if (members[total_members].name.empty())
                {
                    cout << "Name cannot be empty. Try again.\n";
                    continue;
                }

                bool onlyChars = true;
                for (int i = 0; i < members[total_members].name.length(); i++)
                {
                    if (!isalpha(members[total_members].name[i]) && members[total_members].name[i] != ' ')
                    {
                        onlyChars = false;
                        break;
                    }
                }

                if (!onlyChars)
                {
                    cout << "Name must contain only characters and spaces. Try again.\n";
                    continue;
                }

                validName = true;
            }

            bool duplicate = true;

            while (duplicate)
            {
                cout << "2. User ID: ";
                getline(cin, members[total_members].member_ID);

                if (members[total_members].member_ID.empty())
                {
                    cout << "User ID cannot be empty. Try again.\n";
                    continue;
                }

                bool valid = true;
                for (char m : members[total_members].member_ID)
                {
                    if (!isalnum(m))
                    {
                        valid = false;
                        break;
                    }
                }

                if (!valid)
                {
                    cout << "Book ID must contain only letters and digits.\n";
                    continue;
                }

                duplicate = false;
                for (int i = 0; i < total_members; i++)
                {
                    if (members[i].member_ID == members[total_members].member_ID)
                    {
                        duplicate = true;
                        cout << "User ID already exists. Try another.\n";
                        break;
                    }
                }
            }

            // Validate Department - only IT, CS, SE, DS, AI, CY
            bool validDept = false;
            while (!validDept)
            {
                cout << "3. Department (IT, CS, SE, DS, AI, CY): ";
                getline(cin, members[total_members].department);

                if (members[total_members].department.empty())
                {
                    cout << "Department cannot be empty. Try again.\n";
                    continue;
                }

                if (members[total_members].department == "IT" || members[total_members].department == "CS" ||
                    members[total_members].department == "SE" || members[total_members].department == "DS" ||
                    members[total_members].department == "AI" || members[total_members].department == "CY")
                {
                    validDept = true;
                }
                else
                {
                    cout << "Invalid department. Please enter one of: IT, CS, SE, DS, AI, CY\n";
                }
            }

            // Validate Session - only 4 positive numbers
            bool validSession = false;
            while (!validSession)
            {
                cout << "4. Session (4 digit year, e.g., 2024): ";
                getline(cin, members[total_members].session);

                if (members[total_members].session.empty())
                {
                    cout << "Session cannot be empty. Try again.\n";
                    continue;
                }

                if (members[total_members].session.length() != 4)
                {
                    cout << "Session must be exactly 4 digits. Try again.\n";
                    continue;
                }

                bool allDigits = true;
                for (int i = 0; i < members[total_members].session.length(); i++)
                {
                    if (!isdigit(members[total_members].session[i]))
                    {
                        allDigits = false;
                        break;
                    }
                }

                if (!allDigits)
                {
                    cout << "Session must contain digits only. Try again.\n";
                    continue;
                }

                validSession = true;
            }

            // Validate Contact Number - only positive 11 numbers
            bool validContact = false;
            while (!validContact)
            {
                cout << "5. Contact number (11 digits): ";
                string contact;
                getline(cin, contact);

                if (contact.empty())
                {
                    cout << "Contact cannot be empty. Try again.\n";
                    continue;
                }

                if (contact.length() != 11)
                {
                    cout << "Contact must be exactly 11 digits long. Try again.\n";
                    continue;
                }

                bool allDigits = true;
                for (int i = 0; i < contact.length(); i++)
                {
                    if (!isdigit(contact[i]))
                    {
                        allDigits = false;
                        break;
                    }
                }

                if (!allDigits)
                {
                    cout << "Contact must contain digits only. Try again.\n";
                    continue;
                }

                if (contact[0] < '0')
                {
                    cout << "Contact number must be positive. Try again.\n";
                    continue;
                }

                members[total_members].contact = contact;
                validContact = true;
            }

            validInput = true;
        }

        file << members[total_members].name << "," << members[total_members].member_ID << ","
             << members[total_members].department << "," << members[total_members].session << ","
             << members[total_members].contact << "\n";

        total_members++;
        cout << "Member added! Total: " << total_members << endl;

        cout << "Add another? (y/n): ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } while (choice == 'y' || choice == 'Y');

    file.close();
}

void syncFile()
{
    ofstream file("member.csv");
    file << "Name,ID,Department,Session,Contact\n";
    for (int i = 0; i < total_members; i++)
    {
        file << members[i].name << "," << members[i].member_ID << ","
             << members[i].department << "," << members[i].session << ","
             << members[i].contact << "\n";
    }
    file.close();
}

void deleteMember()
{
    char choice;
    do
    {
        string delete_ID;
        cout << "Enter Member ID to delete: ";
        getline(cin, delete_ID);

        bool found = false;
        int index = -1;
        for (int i = 0; i < total_members; i++)
        {
            if (members[i].member_ID == delete_ID)
            {
                found = true;
                index = i;
                break;
            }
        }

        if (found)
        {

            for (int i = index; i < total_members - 1; i++)
            {
                members[i] = members[i + 1];
            }
            total_members--;
            syncFile();
            cout << "Member Deleted Successfully!\n";
        }
        else
        {
            cout << "Member not found!\n";
        }

        cout << "Delete another? (y/n): ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } while (choice == 'y' || choice == 'Y');
}

void searchMemberByID()
{
    char choice;
    do
    {
        string searchID;
        cout << "Enter Member ID to search: ";
        getline(cin, searchID);

        bool found = false;
        for (int i = 0; i < total_members; i++)
        {
            if (members[i].member_ID == searchID)
            {
                cout << "\nMember found:\n";
                cout << "Name: " << members[i].name << endl;
                cout << "ID: " << members[i].member_ID << endl;
                cout << "Department: " << members[i].department << endl;
                cout << "Session: " << members[i].session << endl;
                cout << "Contact: " << members[i].contact << endl;
                found = true;
                break;
            }
        }

        if (!found)
        {
            cout << "No member found with ID '" << searchID << "'.\n";
        }

        cout << "Search again? (y/n): ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } while (choice == 'y' || choice == 'Y');
}

void displayMembers()
{
    if (total_members == 0)
    {
        cout << "No members found.\n";
        return;
    }

    cout << "\n\t\t=== MEMBER LIST ===\n\n";
    cout << left << setw(20) << "Name" << " | "
         << setw(12) << "ID" << " | "
         << setw(15) << "Department" << " | "
         << setw(10) << "Session" << " | "
         << setw(15) << "Contact" << endl;
    cout << string(75, '-') << endl;

    for (int i = 0; i < total_members; i++)
    {
        cout << left << setw(20) << members[i].name << " | "
             << setw(12) << members[i].member_ID << " | "
             << setw(15) << members[i].department << " | "
             << setw(10) << members[i].session << " | "
             << setw(15) << members[i].contact << endl;
    }

    cout << "\nTotal members: " << total_members << endl;
}

void manage_members()
{

    members = new Member[max_members];
    loadMembersFromFile();

    int choice;
    do
    {
        displayMenu();
        cout << "Enter choice: ";

        // Validate input
        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number between 1 and 5.\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Check if choice is in valid range
        if (choice < 1 || choice > 5)
        {
            cout << "Invalid choice! Please enter a number between 1 and 5.\n";
            continue;
        }

        switch (choice)
        {
        case 1:
            system("cls");
            cout << "\n=== ADD MEMBER ===\n";
            addMember();
            break;
        case 2:
            system("cls");
            cout << "\n=== SEARCH MEMBER ===\n";
            searchMemberByID();
            break;
        case 3:
            system("cls");
            cout << "\n=== DELETE MEMBER ===\n";
            deleteMember();
            break;
        case 4:
            system("cls");
            cout << "\n=== DISPLAY MEMBERS ===\n";
            displayMembers();
            break;
        case 5:
            system("cls");
            cout << "Exiting...\n";
            break;
        }
    } while (choice != 5);

    delete[] members;
}