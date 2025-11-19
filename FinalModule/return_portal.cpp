#include "return.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <algorithm>
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
    int days = d.day;
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    for (int y = 1900; y < d.year; y++)
    {
        days += isLeap(y) ? 366 : 365;
    }
    for (int m = 1; m < d.month; m++)
    {
        if (m == 2 && isLeap(d.year))
            days += 29;
        else
            days += daysInMonth[m];
    }
    return days;
}

void markReturnedInHistory(string bookID, string memberID, string issueDateStr)
{
    ifstream inFile("history.csv");
    ofstream outFile("history_temp.csv");

    if (!inFile.is_open() || !outFile.is_open())
    {
        cout << "Error opening history file.\n";
        return;
    }

    string line;
    getline(inFile, line);
    outFile << line << "\n";

    bool found = false;

    while (getline(inFile, line))
    {
        stringstream ss(line);
        string segment;

        // Use DMA array instead of vector
        char **col = nullptr;
        int colCount = 0;
        int colCapacity = 10;
        col = new char *[colCapacity];

        while (getline(ss, segment, ','))
        {
            if (colCount >= colCapacity)
            {
                char **newCol = new char *[colCapacity * 2];
                for (int i = 0; i < colCount; i++)
                {
                    newCol[i] = col[i];
                }
                delete[] col;
                col = newCol;
                colCapacity *= 2;
            }
            col[colCount] = new char[segment.length() + 1];
            strcpy(col[colCount], segment.c_str());
            colCount++;
        }

        if (colCount >= 6)
        {
            if (string(col[0]) == memberID && string(col[3]) == bookID && string(col[4]) == issueDateStr && string(col[5]) == "NO")
            {
                outFile << col[0] << "," << col[1] << "," << col[2] << "," << col[3] << "," << col[4] << ",YES\n";
                found = true;
            }
            else
            {
                outFile << line << "\n";
            }
        }

        // Cleanup DMA
        for (int i = 0; i < colCount; i++)
        {
            delete[] col[i];
        }
        delete[] col;
    }

    inFile.close();
    outFile.close();

    remove("history.csv");
    rename("history_temp.csv", "history.csv");

    if (found)
        cout << "History updated successfully!.\n";
    else
        cout << "No matching history record found or already marked returned.\n";
}

bool removeIssued(string bookID, string memberID)
{
    ifstream inFile("issue.csv");
    ofstream outFile("issue_temp.csv");
    if (!inFile.is_open() || !outFile.is_open())
    {
        cout << "Error opening issue files.\n";
        return false;
    }

    bool removed = false;
    string line;

    // Preserve header if present
    if (getline(inFile, line))
    {
        outFile << line << "\n";
    }

    while (getline(inFile, line))
    {
        stringstream ss(line);
        string segment;

        // Use DMA array instead of multiple string variables
        char **col = nullptr;
        int colCount = 0;
        int colCapacity = 5;
        col = new char *[colCapacity];

        while (getline(ss, segment, ','))
        {
            if (colCount >= colCapacity)
            {
                char **newCol = new char *[colCapacity * 2];
                for (int i = 0; i < colCount; i++)
                {
                    newCol[i] = col[i];
                }
                delete[] col;
                col = newCol;
                colCapacity *= 2;
            }
            col[colCount] = new char[segment.length() + 1];
            strcpy(col[colCount], segment.c_str());
            colCount++;
        }

        if (colCount >= 5 && string(col[3]) == bookID && string(col[0]) == memberID)
        {
            removed = true;
        }
        else
        {
            outFile << line << "\n";
        }

        // Cleanup DMA
        for (int i = 0; i < colCount; i++)
        {
            delete[] col[i];
        }
        delete[] col;
    }

    inFile.close();
    outFile.close();

    remove("issue.csv");
    rename("issue_temp.csv", "issue.csv");

    if (removed)
        cout << "Book removed from issue.csv.\n";
    else
        cout << "Book was not found in issue.csv (may have been issued, but not to this member).\n";

    return removed;
}

bool bookExists(string bookID, string bookTitle)
{
    ifstream file("issue.csv");
    string line;
    getline(file, line); // skip header

    while (getline(file, line))
    {
        stringstream ss(line);
        string segment;

        // Use DMA array instead of multiple string variables
        char **col = nullptr;
        int colCount = 0;
        int colCapacity = 5;
        col = new char *[colCapacity];

        while (getline(ss, segment, ','))
        {
            if (colCount >= colCapacity)
            {
                char **newCol = new char *[colCapacity * 2];
                for (int i = 0; i < colCount; i++)
                {
                    newCol[i] = col[i];
                }
                delete[] col;
                col = newCol;
                colCapacity *= 2;
            }
            col[colCount] = new char[segment.length() + 1];
            strcpy(col[colCount], segment.c_str());
            colCount++;
        }

        if (colCount >= 5)
        {
            if (string(col[3]) == bookID || string(col[1]) == bookTitle)
            {
                // Cleanup DMA
                for (int i = 0; i < colCount; i++)
                {
                    delete[] col[i];
                }
                delete[] col;
                file.close();
                return true;
            }
        }

        // Cleanup DMA
        for (int i = 0; i < colCount; i++)
        {
            delete[] col[i];
        }
        delete[] col;
    }
    file.close();
    return false;
}

void addFine(string bookID, string memberID, int daysLate, int fineAmount, Date issueDate, Date returnDate)
{
    ofstream file("fine.csv", ios::app);
    if (!file.is_open())
    {
        cout << "Error opening fine.csv for writing.\n";
        return;
    }
    file << bookID << "," << memberID << "," << issueDate.day << "-" << issueDate.month << "-" << issueDate.year << "," << returnDate.day << "-" << returnDate.month << "-" << returnDate.year << "," << daysLate << "," << fineAmount << "\n";
    file.close();
}

// Function to toggle book status in books.csv from YES to NO
bool returnBookToInventory(string bookID, string bookTitle, string bookAuthor)
{
    ifstream inFile("books.csv");
    ofstream outFile("books_temp.csv");

    if (!inFile.is_open() || !outFile.is_open())
    {
        cout << "Error opening books.csv.\n";
        return false;
    }

    string line;
    bool found = false;

    // Copy header
    getline(inFile, line);
    outFile << line << "\n";

    while (getline(inFile, line))
    {
        stringstream ss(line);
        string segment;

        // Use DMA array instead of multiple string variables
        char **col = nullptr;
        int colCount = 0;
        int colCapacity = 10;
        col = new char *[colCapacity];

        while (getline(ss, segment, ','))
        {
            if (colCount >= colCapacity)
            {
                char **newCol = new char *[colCapacity * 2];
                for (int i = 0; i < colCount; i++)
                {
                    newCol[i] = col[i];
                }
                delete[] col;
                col = newCol;
                colCapacity *= 2;
            }
            col[colCount] = new char[segment.length() + 1];
            strcpy(col[colCount], segment.c_str());
            colCount++;
        }

        // Check if this is the book being returned
        if (colCount >= 4 && string(col[0]) == bookID && string(col[3]) == "YES")
        {
            // Toggle status from YES to NO
            outFile << col[0] << "," << col[1] << "," << col[2] << ",NO\n";
            found = true;
        }
        else
        {
            outFile << line << "\n";
        }

        // Cleanup DMA
        for (int i = 0; i < colCount; i++)
        {
            delete[] col[i];
        }
        delete[] col;
    }

    inFile.close();
    outFile.close();

    if (found)
    {
        remove("books.csv");
        rename("books_temp.csv", "books.csv");
        cout << "Book status updated.\n";
        return true;
    }
    else
    {
        remove("books_temp.csv");
        cout << "Book not found in books.csv or already returned.\n";
        return false;
    }
}
// Function to retrieve issue date, title, and author from issue.csv for a given book and member
bool getIssueDateFromIssue(string bookID, string memberID, string &issueDateStr, string &bookTitle, string &bookAuthor)
{
    ifstream file("issue.csv");
    if (!file.is_open())
    {
        cout << "Error opening issue.csv.\n";
        return false;
    }

    string line;
    getline(file, line); // skip header

    while (getline(file, line))
    {
        stringstream ss(line);
        string segment;

        // Use DMA array instead of multiple string variables
        char **col = nullptr;
        int colCount = 0;
        int colCapacity = 5;
        col = new char *[colCapacity];

        while (getline(ss, segment, ','))
        {
            if (colCount >= colCapacity)
            {
                char **newCol = new char *[colCapacity * 2];
                for (int i = 0; i < colCount; i++)
                {
                    newCol[i] = col[i];
                }
                delete[] col;
                col = newCol;
                colCapacity *= 2;
            }
            col[colCount] = new char[segment.length() + 1];
            strcpy(col[colCount], segment.c_str());
            colCount++;
        }

        if (colCount >= 5 && string(col[0]) == memberID && string(col[3]) == bookID)
        {
            issueDateStr = string(col[4]);
            bookTitle = string(col[1]);
            bookAuthor = string(col[2]);

            // Cleanup DMA
            for (int i = 0; i < colCount; i++)
            {
                delete[] col[i];
            }
            delete[] col;
            file.close();
            return true;
        }

        // Cleanup DMA
        for (int i = 0; i < colCount; i++)
        {
            delete[] col[i];
        }
        delete[] col;
    }

    file.close();
    return false;
}

void returnBook()
{
    string bookID, memberID, issueStr, returnStr, again;
    string bookTitle, bookAuthor;
    Date issue, ret;

    cout << "\n\n\t\t\t=== Library Book Return Portal ===\n\n";

    do
    {
        cout << "Enter Book ID: ";
        if (!(cin >> bookID))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please try again.\n";
            continue;
        }

        cout << "Enter Member ID: ";
        if (!(cin >> memberID))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please try again.\n";
            continue;
        }

        // Check if book exists in issue.csv
        if (!bookExists(bookID, ""))
        {
            cout << "Book ID not found in issue.csv. Cannot process return.\n";
            continue;
        }

        // Retrieve issue date, title, and author from issue.csv
        if (!getIssueDateFromIssue(bookID, memberID, issueStr, bookTitle, bookAuthor))
        {
            cout << "No pending issue record found for this Book ID and Member ID in issue.csv.\n";
            continue;
        }

        issue = parseDate(issueStr);
        if (!validDate(issue))
        {
            cout << "Invalid issue date found. Cannot proceed.\n";
            continue;
        }

        cout << "Issue Date retrieved from issue.csv: " << issueStr << "\n";

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
                cout << "Return date cannot be before issue date.\n";
                continue;
            }
            break;
        }

        // Remove from issue.csv
        bool wasRemoved = removeIssued(bookID, memberID);

        if (wasRemoved)
        {
            // Return book to books.csv inventory
            returnBookToInventory(bookID, bookTitle, bookAuthor);

            // Update history.csv with return status YES
            markReturnedInHistory(bookID, memberID, issueStr);
        }
        else
        {
            cout << "Book return process stopped because it was not listed as issued to this member.\n";
            continue;
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

    } while (again == "y" || again == "Y");
}

// Wrapper function to match declaration in final.cpp
void return_books()
{
    returnBook();
}