
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
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

void resizeArray()
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
    cout << "\n\n\t\t\t=== MEMBER PORTAL (DYNAMIC MEMORY!) ===\n\n";
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
    ofstream file("member.csv", ios::app);
    if (!file)
    {
        cout << "Could not open file!\n";
        return;
    }

    file.seekp(0, ios::end);
    if (file.tellp() == 0)
    {
        file << "Name,ID,Department,Session,Contact\n";
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
            cout << "1. Name: ";
            getline(cin, members[total_members].name);

            bool duplicate = true;

            while (duplicate)
            {
                cout << "2. User ID: ";
                getline(cin, members[total_members].member_ID);
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

            cout << "3. Department: ";
            getline(cin, members[total_members].department);
            cout << "4. Session: ";
            getline(cin, members[total_members].session);
            cout << "5. Contact: ";
            getline(cin, members[total_members].contact);

            if (members[total_members].name.empty() || members[total_members].member_ID.empty() ||
                members[total_members].department.empty() || members[total_members].session.empty() ||
                members[total_members].contact.empty())
            {
                cout << "\nAll fields must be filled!\n";
            }
            else
            {
                validInput = true;
            }
        }

        file << members[total_members].name << "," << members[total_members].member_ID << ","
             << members[total_members].department << "," << members[total_members].session << ","
             << members[total_members].contact << "\n";

        total_members++;
        cout << "Member added! Total: " << total_members << endl;

        cout << "Add another? (y/n): ";
        cin >> choice;
        cin.ignore();
    } while (choice == 'y' || choice == 'Y');

    file.close();
}
void syncFile() {
    ofstream file("member.csv");
    file << "Name,ID,Department,Session,Contact\n";
    for (int i = 0; i < total_members; i++) {
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
        cin.ignore();
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
        cin.ignore();
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

int main()
{

    members = new Member[max_members];
    loadMembersFromFile();

    int choice;
    do
    {
        displayMenu();
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
            cout << "\n=== ADD MEMBER ===\n";
            addMember();
            break;
        case 2:
            cout << "\n=== SEARCH MEMBER ===\n";
            searchMemberByID();
            break;
        case 3:
            cout << "\n=== DELETE MEMBER ===\n";
            deleteMember();
            break;
        case 4:
            cout << "\n=== DISPLAY MEMBERS ===\n";
            displayMembers();
            break;
        case 5:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice!\n";
        }
        if(choice<1&&choice>5)
        {
            cout<<"invalide choice!"<<endl;
        }
    } while (choice != 5);

    delete[] members;
    return 0;
}