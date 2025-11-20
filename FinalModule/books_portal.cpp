#include "books.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <limits>
using namespace std;

struct Book
{
    string book_ID;
    string title;
    string author;
    string issue_status;
};

Book *books = nullptr;
int bookCount = 0;
int capacity = 0;

void ensureCapacity(int minCapacity)
{
    if (capacity >= minCapacity)
        return;

    int newCapacity = capacity > 0 ? capacity * 2 : 4;
    while (newCapacity < minCapacity)
        newCapacity *= 2;

    Book *newArr = new Book[newCapacity];
    for (int i = 0; i < bookCount; ++i)
        newArr[i] = books[i];

    delete[] books;
    books = newArr;
    capacity = newCapacity;
}

void displayBooksMenu()
{
    cout << "\n\n\t\t\t=== Book Management System ===\n\n";
    cout << "What do you want to do: \n";
    cout << "1. Add a book. \n";
    cout << "2. Search a book. \n";
    cout << "3. Delete a book. \n";
    cout << "4. Display all books.\n";
    cout << "5. Exit. \n";
}

void loadBooksFromFile()
{
    ifstream file("books.csv");
    if (!file)
    {
        cout << "No existing file found. A new one will be created.\n";
        return;
    }

    string line;
    getline(file, line);

    while (getline(file, line))
    {
        if (!line.empty())
        {
            stringstream ss(line);
            Book book;
            string token;

            getline(ss, book.book_ID, ',');
            getline(ss, book.title, ',');
            getline(ss, book.author, ',');
            getline(ss, book.issue_status);

            ensureCapacity(bookCount + 1);
            books[bookCount++] = book;
        }
    }

    file.close();
}

void addBook()
{
    ofstream file("books.csv", ios::app);
    if (!file)
    {
        cout << "Could not open file!\n";
        return;
    }

    file.seekp(0, ios::end);
    if (file.tellp() == 0)
    {
        file << "ID,Title,Author,Issue_Status\n";
    }

    char choice;
    do
    {
        Book book;
        bool validInput = false;

        while (!validInput)
        {
            cout << "Enter desired ID for the book: ";
            getline(cin, book.book_ID);

            bool valid = true;
            for (char c : book.book_ID)
            {
                if (!isalnum(c))
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

            bool duplicate = false;
            for (int i = 0; i < bookCount; i++)
            {
                if (books[i].book_ID == book.book_ID)
                {
                    duplicate = true;
                    break;
                }
            }

            if (duplicate)
            {
                cout << "ID '" << book.book_ID << "' already exists. Try another.\n";
                continue;
            }

            cout << "Enter book title: ";
            getline(cin, book.title);

            cout << "Enter book author: ";
            getline(cin, book.author);

            book.issue_status = "NO";

            if (book.book_ID.empty() || book.title.empty() || book.author.empty())
            {
                cout << "\nAll fields must be filled! Please try again.\n\n";
            }
            else
            {
                validInput = true;
            }
        }

        file << book.book_ID << "," << book.title << "," << book.author << "," << "NO" << "\n";
        ensureCapacity(bookCount + 1);
        books[bookCount++] = book;
        cout << "Book added successfully with ID '" << book.book_ID << "'.\n";

        cout << "Do you want to add another book? (y/n): ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

    } while (choice == 'y' || choice == 'Y');

    file.close();
}

void deleteBook()
{
    char choice;
    do
    {
        string delete_ID;
        cout << "Enter book ID to delete: ";
        getline(cin, delete_ID);

        if (delete_ID.empty())
        {
            cout << "\nID cannot be empty! Please try again.\n\n";
            continue;
        }

        bool found = false;
        ofstream tempFile("temp.csv");
        ifstream originalFile("books.csv");

        if (!originalFile || !tempFile)
        {
            cout << "Could not open file.\n";
            return;
        }

        string line;
        getline(originalFile, line);
        tempFile << line << '\n';

        while (getline(originalFile, line))
        {
            stringstream ss(line);
            string id;
            getline(ss, id, ',');

            if (id != delete_ID)
            {
                tempFile << line << '\n';
            }
            else
            {
                found = true;
            }
        }

        originalFile.close();
        tempFile.close();

        if (found)
        {
            remove("books.csv");
            rename("temp.csv", "books.csv");
            cout << "Book Deleted Successfully!\n";

            for (int i = 0; i < bookCount; i++)
            {
                if (books[i].book_ID == delete_ID)
                {

                    for (int j = i; j + 1 < bookCount; ++j)
                        books[j] = books[j + 1];
                    --bookCount;
                    break;
                }
            }
        }
        else
        {
            remove("temp.csv");
            cout << "Book not found!\n";
        }

        cout << "\nDo you want to delete another book? (y/n): ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } while (choice == 'y' || choice == 'Y');
}

void searchBookByID()
{
    char choice;
    do
    {
        string searchID;
        cout << "Enter book ID to search: ";
        getline(cin, searchID);

        if (searchID.empty())
        {
            cout << "\nID cannot be empty! Please try again.\n\n";
            continue;
        }

        bool found = false;
        for (int i = 0; i < bookCount; i++)
        {
            if (books[i].book_ID == searchID)
            {
                cout << "\nBook found:\n";
                cout << "ID: " << books[i].book_ID << endl;
                cout << "Title: " << books[i].title << endl;
                cout << "Author: " << books[i].author << endl;
                cout << "Issue_Status: " << books[i].issue_status << endl;
                found = true;
                break;
            }
        }

        if (!found)
        {
            cout << "No book found with ID '" << searchID << "'.\n";
        }

        cout << "\nDo you want to search again? (y/n): ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } while (choice == 'y' || choice == 'Y');
}

void displayBooks()
{
    if (bookCount == 0)
    {
        cout << "No books found.\n";
        return;
    }

    cout << "\n\n\t\t\t=== Book List ===\n\n";
    cout << left << setw(10) << "Book ID" << " | "
         << setw(30) << "Title" << " | "
         << setw(20) << "Author" << " | "
         << setw(20) << "Issue_Status" << endl;
    cout << "--------------------------------------------------------------------------------------------------\n";

    for (int i = 0; i < bookCount; i++)
    {
        cout << left << setw(10) << books[i].book_ID << " | "
             << setw(30) << books[i].title << " | "
             << setw(20) << books[i].author << " | "
             << setw(20) << books[i].issue_status << endl;
    }

    cout << "\nTotal books: " << bookCount << endl;
}

void manage_books()
{
    loadBooksFromFile();

    int choice;
    do
    {
        displayBooksMenu();
        cout << "Enter your choice: ";

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
            cout << "\n\n\t\t\t=== Add a Book ===\n\n";
            addBook();
            break;
        case 2:
            system("cls");
            cout << "\n\n\t\t\t=== Search a Book ===\n\n";
            searchBookByID();
            break;
        case 3:
            system("cls");
            cout << "\n\n\t\t\t=== Delete a Book ===\n\n";
            deleteBook();
            break;
        case 4:
            system("cls");
            cout << "\n\n\t\t\t=== Display All Books ===\n\n";
            displayBooks();
            break;
        case 5:
            cout << "Exiting...\n\n";
            system("cls");
            return;
        }
    } while (choice != 5);
}