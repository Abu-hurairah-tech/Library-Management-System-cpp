#include "return.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <limits>
using namespace std;

struct Date
{
    int day, month, year;
};

Date parseDate(string s)
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

bool isLeap(int y)
{
    return (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0));
}

bool validDate(Date d)
{
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isLeap(d.year))
        daysInMonth[2] = 29;
    if (d.month < 1 || d.month > 12 || d.day < 1 || d.day > daysInMonth[d.month])
        return false;
    return true;
}

int dateToDays(Date d)
{
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int days = d.year * 365 + d.day;
    for (int i = 1; i < d.month; i++)
        days += daysInMonth[i];
    days += d.year / 4 - d.year / 100 + d.year / 400;
    return days;
}

bool findIssued(string bookID, string memberID, Date &issueDate, string &title, string &author)
{
    ifstream f("issue.csv");
    if (!f)
    {
        cout << "issue.csv not found.\n";
        return false;
    }
    string line;
    getline(f, line); // skip header
    while (getline(f, line))
    {
        stringstream ss(line);
        string mid, btitle, auth, bid, dateStr;
        getline(ss, mid, ',');
        getline(ss, btitle, ',');
        getline(ss, auth, ',');
        getline(ss, bid, ',');
        getline(ss, dateStr, ',');
        if (mid == memberID && bid == bookID)
        {
            issueDate = parseDate(dateStr);
            title = btitle;
            author = auth.empty() ? "Unknown" : auth;
            return true;
        }
    }
    return false;
}

void removeIssued(string bookID, string memberID)
{
    ifstream in("issue.csv");
    ofstream out("temp.csv");
    string line;
    getline(in, line);
    out << line << "\n";
    while (getline(in, line))
    {
        stringstream ss(line);
        string mid, btitle, auth, bid, date;
        getline(ss, mid, ',');
        getline(ss, btitle, ',');
        getline(ss, auth, ',');
        getline(ss, bid, ',');
        getline(ss, date, ',');
        if (mid != memberID || bid != bookID)
            out << line << "\n";
    }
    in.close();
    out.close();
    remove("issue.csv");
    rename("temp.csv", "issue.csv");
}

bool bookExists(string id, string title)
{
    ifstream f("books.csv");
    if (!f)
        return false;
    string line;
    getline(f, line);
    while (getline(f, line))
    {
        stringstream ss(line);
        string bid, btitle;
        getline(ss, bid, ',');
        getline(ss, btitle, ',');
        if (bid == id && btitle == title)
            return true;
    }
    return false;
}

void addFine(string bookID, string memberID, int daysLate, int fine, Date issue, Date ret)
{
    bool fileExists = ifstream("fine.csv").good();
    ofstream f("fine.csv", ios::app);
    if (!fileExists)
        f << "BookID,MemberID,DateOfIssue,DateOfReturn,DaysLate,Fine\n";
    f << bookID << "," << memberID << ","
      << setw(2) << setfill('0') << issue.day << "-" << setw(2) << issue.month << "-" << issue.year << ","
      << setw(2) << ret.day << "-" << setw(2) << ret.month << "-" << ret.year << ","
      << daysLate << "," << fine << "\n";
}

void returnBook()
{
    string bookID, memberID, returnStr, title, author;
    char again;
    Date issue, ret;

    cout << "\n=== Return Books ===\n";
    do
    {
        cout << "Enter Book ID: ";
        if (!(cin >> bookID))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please try again.\n";
            continue;
        }

        cout << "Enter Member ID: ";
        if (!(cin >> memberID))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please try again.\n";
            continue;
        }

        if (!findIssued(bookID, memberID, issue, title, author))
        {
            cout << "This book was not issued to this member.\n";
            cout << "Try again? (y/n): ";
            cin >> again;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore();
        while (true)
        {
            cout << "Enter Return Date (DD-MM-YYYY): ";
            getline(cin, returnStr);
            ret = parseDate(returnStr);
            if (!validDate(ret))
            {
                cout << "Invalid date.\n";
                continue;
            }
            if (dateToDays(ret) < dateToDays(issue))
            {
                cout << "Return date before issue date.\n";
                continue;
            }
            break;
        }

        removeIssued(bookID, memberID);

        if (!bookExists(bookID, title))
        {
            ofstream b("books.csv", ios::app);
            b << bookID << "," << title << "," << author << "\n";
            cout << "Book restored to books.csv.\n";
        }

        int days = dateToDays(ret) - dateToDays(issue);
        cout << "Days between issue and return: " << days << endl;

        if (days <= 7)
            cout << "No fine. Book returned on time.\n";
        else
        {
            int fine = (days - 7) * 100;
            addFine(bookID, memberID, days - 7, fine, issue, ret);
            cout << "Late return! Fine = Rs." << fine << endl;
        }

        cout << "\nReturn another book? (y/n): ";
        cin >> again;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

    } while (again == 'y' || again == 'Y');
}

void return_books()
{
    returnBook();
}
