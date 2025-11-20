#include "issue.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <iostream>
#include <limits>
#include <cstring>
using namespace std;

struct Date
{
    int day, month, year;
};

Date parse_Date(string s)
{
    Date d = {0, 0, 0};
    char c;
    stringstream ss(s);
    if (ss >> d.day >> c >> d.month >> c >> d.year)
    {
    }
    else
    {
        ss.clear();
        ss.str(s);
        ss >> d.day >> d.month >> d.year;
    }
    if (d.year < 100)
        d.year += 2000;
    return d;
}

bool is_Leap(int y)
{
    return (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0));
}

bool valid_Date(Date d)
{
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (is_Leap(d.year))
        daysInMonth[2] = 29;
    if (d.month < 1 || d.month > 12 || d.day < 1 || d.day > daysInMonth[d.month])
        return false;
    return true;
}

void format_date(const Date &d, string &buffer)
{
    ostringstream oss;
    oss << setfill('0') << setw(2) << d.day << "-"
        << setfill('0') << setw(2) << d.month << "-"
        << setw(4) << d.year;
    buffer = oss.str();
}

void input_valid_date(Date &d)
{
    string input;
    bool valid = false;
    while (!valid)
    {
        cout << "Enter date of issue (DD-MM-YYYY): ";
        getline(cin, input);
        d = parse_Date(input);
        if (!valid_Date(d))
        {
            cout << "Invalid date. Please enter a valid calendar date.\n";
            continue;
        }
        valid = true;
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

Member *issue_members = nullptr;
int issue_total_members = 0;
int issue_max_members = 10;

static void resizeArray()
{
    issue_max_members *= 2;
    Member *newArray = new Member[issue_max_members];
    for (int i = 0; i < issue_total_members; i++)
    {
        newArray[i] = issue_members[i];
    }
    delete[] issue_members;
    issue_members = newArray;
}

void loadMembersFromFileIssue()
{
    ifstream file("member.csv");
    if (!file)
    {
        cout << "No member file found.\n";
        return;
    }
    string line;
    getline(file, line); // skip header
    issue_total_members = 0;
    while (getline(file, line))
    {
        if (issue_total_members >= issue_max_members)
            resizeArray();
        stringstream ss(line);
        getline(ss, issue_members[issue_total_members].name, ',');
        getline(ss, issue_members[issue_total_members].member_ID, ',');
        getline(ss, issue_members[issue_total_members].department, ',');
        getline(ss, issue_members[issue_total_members].session, ',');
        getline(ss, issue_members[issue_total_members].contact);
        issue_total_members++;
    }
    file.close();
}

bool member_verification(string &id_verify)
{
    cout << "Enter member ID to verify: ";
    getline(cin, id_verify);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    for (int i = 0; i < issue_total_members; i++)
    {
        if (issue_members[i].member_ID == id_verify)
        {
            cout << "Member found: " << issue_members[i].name << endl;
            return true;
        }
    }
    cout << "Member not found!\n";
    return false;
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
        ofstream temp("books_temp.csv");

        if (!stock || !issue_file || !history || !temp)
        {
            cout << "Unable to open file!\n";
            return 1;
        }

        // Write headers if files are empty
        issue_file.seekp(0, ios::end);
        if (issue_file.tellp() == 0)
            issue_file << "Member ID,Book Title,Book Author,Book ID,Issue Date\n";

        history.seekp(0, ios::end);
        if (history.tellp() == 0)
            history << "Member ID,Book Title,Book Author,Book ID,Issue Date,Return_Status\n";

        // Copy header from books.csv
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
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
        bool already_issued = false;

        while (getline(stock, line))
        {
            stringstream ss(line);
            string bid, title, author, status;

            getline(ss, bid, ',');
            getline(ss, title, ',');
            getline(ss, author, ',');
            getline(ss, status); 

            if (bid == book_id)
            {
                if (status == "YES")
                {
                    already_issued = true;
                    temp << line << "\n";
                }
                else
                {
                    cout << "Book available: " << title << " by " << author << "\n";
                    found = true;

                    string member_name = "Unknown";
                    for (int i = 0; i < issue_total_members; i++)
                    {
                        if (issue_members[i].member_ID == id_verify)
                        {
                            member_name = issue_members[i].name;
                            break;
                        }
                    }

                    Date issue_date;
                    input_valid_date(issue_date);
                    string date_str;
                    format_date(issue_date, date_str);

                    cout << "Book issued to: " << member_name << ", " << id_verify << "\n";

                    // Add to issue.csv and history.csv
                    issue_file << id_verify << "," << title << "," << author << "," << book_id << "," << date_str << "\n";
                    history << id_verify << "," << title << "," << author << "," << book_id << "," << date_str << ",NO\n";

                    // Update status to YES in books.csv
                    temp << bid << "," << title << "," << author << ",YES\n";
                    any_issue_done = true;
                }
            }
            else
            {
                temp << line << "\n";
            }
        }

        if (already_issued)
            cout << "This book is already issued to someone else!\n";
        else if (!found)
            cout << "Book not found or invalid ID.\n";

        stock.close();
        issue_file.close();
        history.close();
        temp.close();

        if (any_issue_done)
        {
            remove("books.csv");
            rename("books_temp.csv", "books.csv");
        }
        else
        {
            remove("books_temp.csv");
        }

        cout << "Do you want to issue another book? (y/n): ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

    } while (choice == 'y' || choice == 'Y');

    return 0;
}

void issue_history()
{
    string id;
    cout << "Enter Member ID to view history: ";
    getline(cin, id);

    ifstream history("history.csv");
    if (!history.is_open())
    {
        cout << "No issue history found.\n";
        return;
    }

    string line;
    getline(history, line); // skip header
    bool found = false;

    cout << "\n=== Issue History for Member ID: " << id << " ===\n";
    cout << left << setw(30) << "Title" << setw(30) << "Author"
         << setw(15) << "Issue Date" << setw(15) << "Return_Status" << endl;
    cout << string(90, '-') << endl;

    while (getline(history, line))
    {
        stringstream ss(line);
        string member_id, title, author, book_id, date, return_status;

        getline(ss, member_id, ',');
        getline(ss, title, ',');
        getline(ss, author, ',');
        getline(ss, book_id, ',');
        getline(ss, date, ',');
        getline(ss, return_status);

        if (member_id == id)
        {
            cout << left << setw(30) << title
                 << setw(30) << author
                 << setw(15) << date
                 << setw(15) << return_status << endl;
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

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice < 1 || choice > 3)
        {
            cout << "Invalid choice! Try again.\n";
            continue;
        }

        switch (choice)
        {
        case 1:
            system("cls");
            issue();
            break;
        case 2:
            system("cls");
            issue_history();
            break;
        case 3:
            cout << "Exiting issue portal.\n";
            system("cls");
            return;
        }
    } while (true);
}

void issuing_books()
{
    issue_members = new Member[issue_max_members];
    loadMembersFromFileIssue();
    system("cls");
    issue_books();
    delete[] issue_members;
}