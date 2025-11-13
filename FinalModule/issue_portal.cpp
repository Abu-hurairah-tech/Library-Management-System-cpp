#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdio>
using namespace std;

struct Date
{
    int day, month, year;
};

void format_date(const Date &d, string &buffer)
{
    ostringstream oss;
    oss << setfill('0') << setw(2) << d.day << "-"
        << setfill('0') << setw(2) << d.month << "-"
        << setw(4) << d.year;
    buffer = oss.str();
}

void clear_input_buffer()
{
    cin.clear();
    cin.ignore(10000, '\n');
}

bool is_leap_year(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool is_valid_date(int day, int month, int year)
{
    if (year < 1 || month < 1 || month > 12 || day < 1)
        return false;

    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (is_leap_year(year))
        days_in_month[1] = 29;
    return day <= days_in_month[month - 1];
}

void input_valid_date(Date &d)
{
    bool valid = false;
    while (!valid)
    {
        cout << "Enter date of issue (DD MM YYYY): ";
        if (!(cin >> d.day >> d.month >> d.year))
        {
            cout << "Invalid input format. Please enter 3 integers.\n";
            clear_input_buffer();
            continue;
        }
        if (is_valid_date(d.day, d.month, d.year))
            valid = true;
        else
            cout << "Invalid date. Please enter a valid calendar date.\n";
        clear_input_buffer();
    }
}

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
    for (int i = 0; i < total_members; i++)
    {
        newArray[i] = members[i];
    }
    delete[] members;
    members = newArray;
}

void loadMembersFromFile()
{
    ifstream file("member.csv");
    if (!file)
    {
        cout << "No member file found.\n";
        return;
    }
    string line;
    getline(file, line); // skip header
    total_members = 0;
    while (getline(file, line))
    {
        if (total_members >= max_members)
            resizeArray();
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

int application()
{

    char choice;
    
        ofstream file("member.csv", ios::app);
        if (!file)
        {
            cout << "Could not open file!\n";
            return 1;
        }
        file.seekp(0, ios::end);
        if (file.tellp() == 0)
        {
            file << "Name,ID,Department,Session,Contact\n";
        }
        if (total_members >= max_members)
            resizeArray();

        bool validInput = false;
        while (!validInput)
        {
            cout << "1. Name: ";
            getline(cin, members[total_members].name);

            // UNIQUE ID CHECK USING STRUCT
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
                        cout << "User ID already exists. Try another.\n";
                        duplicate = true;
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

            if (members[total_members].name.empty() ||
                members[total_members].member_ID.empty() ||
                members[total_members].department.empty() ||
                members[total_members].session.empty() ||
                members[total_members].contact.empty())
            {
                cout << "\nAll fields must be filled!\n";
            }
            else
            {
                validInput = true;
            }
        }

        // WRITE TO FILE
        file << members[total_members].name << ","
             << members[total_members].member_ID << ","
             << members[total_members].department << ","
             << members[total_members].session << ","
             << members[total_members].contact << "\n";
        file.close();
        total_members++;
        cout << "Membership added! Total: " << total_members << endl;

       
    

    return 0;
}

bool member_verification(string &id_verify)
{
    cout << "Enter member ID to verify: ";
    getline(cin, id_verify);

    bool found = false;
    for (int i = 0; i < total_members; i++)
    {
        if (members[i].member_ID == id_verify)
        {
            cout << "Member found: " << members[i].name << endl;
            found = true;
            break;
        }
    }

    if (!found)
    {
        cout << "You have to buy membership to issue books\n\n";
        cout << "Do you want to buy membership (yes/no): ";
        char membership_choice;
        cin >> membership_choice;
        cin.ignore();

        if (membership_choice == 'y' || membership_choice == 'Y')
        {
            application();
            loadMembersFromFile(); // Sync RAM
        }
        else
        {
            cout << "Unable to issue a book!\n";
            return false;
        }
    }
    return true;
}

int issue()
{
    char choice;
    bool any_issue_done = false;

    do
    {
        ifstream stock("books.csv");
        ofstream issue_file("issue.csv", ios::app);
        ofstream history("history.csv", ios::app);

        if (!stock || !issue_file || !history)
        {
            cout << "Unable to open file!\n";
            return 1;
        }

        issue_file.seekp(0, ios::end);
        if (issue_file.tellp() == 0)
        {
            issue_file << "Member ID,Book Title,Book Author,Book ID,Issue Date\n";
        }
        history.seekp(0, ios::end);
        if (history.tellp() == 0)
        {
            history << "Member ID,Book Title,Book Author,Book ID,Issue Date,Return Status\n";
        }

        ofstream temp("temp.csv");
        if (!temp)
        {
            cout << "Unable to create temp file.\n";
            return 1;
        }

        string header;
        getline(stock, header);
        temp << header << "\n";

        cout << "To issue a book, you have to be a member\n";

        string id_verify;
        if (!member_verification(id_verify))
        {
            cout << "Member verification failed.\n";
            cout << "Do you want to try again? (y/n): ";
            cin >> choice;
            cin.ignore();
            if (choice != 'y' && choice != 'Y')
                break;
            continue;
        }

        cout << "Member verified.\n";

        string book_id;
        cout << "Enter book ID to issue: ";
        getline(cin, book_id);

        string line;
        bool found = false;

        while (getline(stock, line))
        {
            string original_line = line;
            stringstream ss(line);
            string bid, title, author;

            getline(ss, bid, ',');
            getline(ss, title, ',');
            getline(ss, author, ',');

            if (bid == book_id)
            {
                cout << "Book available: " << title << " by " << author << "\n";
                found = true;

                string member_name = "Unknown";
                for (int i = 0; i < total_members; i++)
                {
                    if (members[i].member_ID == id_verify)
                    {
                        member_name = members[i].name;
                        break;
                    }
                }

                Date issue_date;
                input_valid_date(issue_date);
                string date_str;
                format_date(issue_date, date_str);

                cout << "Book issued to: " << member_name << ", " << id_verify << "\n";
                string return_status = "NO";
                issue_file << id_verify << "," << title << "," << author << "," << book_id << "," << date_str << "\n";
                history << id_verify << "," << title << "," << author << "," << book_id << "," << date_str<<","<<return_status<< "\n";
                any_issue_done = true;
            }
            else
            {
                temp << original_line << "\n";
            }
        }

        if (!found)
            cout << "Book not found in stock.\n";

        stock.close();
        issue_file.close();
        history.close();
        temp.close();

        if (any_issue_done)
        {
            remove("books.csv");
            rename("temp.csv", "books.csv");
        }
        else
        {
            remove("temp.csv");
        }

        cout << "Do you want to issue another book? (y/n): ";
        cin >> choice;
        cin.ignore();

    } while (choice == 'y' || choice == 'Y');

    return 0;
}

void issue_history()
{
    string id;
    cout << "Enter Member ID to view history: ";
    getline(cin, id);

    ifstream history("history.csv");
    if (!history)
    {
        cout << "No issue history found.\n";
        return;
    }

    string line;
    getline(history, line);
    bool found = false;

    cout << "\n=== Issue History for Member ID: " << id << " ===\n";
    cout << left << setw(30) << "Title" << setw(30) << "Author" << setw(15) << "Issue Date" << setw(15) << "Return Status"<< endl;
    cout << string(90, '-') << endl;

    while (getline(history, line))
    {
        stringstream ss(line);
        string member_id, title, author, book_id, date,return_status;
        getline(ss, member_id, ',');
        getline(ss, title, ',');
        getline(ss, author, ',');
        getline(ss, book_id, ',');
        getline(ss, date, ',');
        getline(ss,return_status);

        if (member_id == id)
        {
            cout << left << setw(30) << title << setw(30) << author << setw(15) << date <<setw(15) << return_status << endl;
            found = true;
        }
    }

    if (!found)
        cout << "No history found for Member ID " << id << ".\n";
    history.close();
}

void issue_books()
{
    cout << "\n\n\t\t\t=== Issue Portal ===\n";
    int choice;
    do
    {
        cout << "\n1. Issue a book\n";
        cout << "2. View issue history\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
            issue();
            break;
        case 2:
            issue_history();
            break;
        case 3:
            cout << "Exiting issue portal.\n";
            return;
        default:
            cout << "Invalid choice, please try again.\n";
        }
    } while (choice != 3);
}

int main()
{
    members = new Member[max_members];
    loadMembersFromFile(); // Load into RAM

    issue_books();

    delete[] members;
    return 0;
}