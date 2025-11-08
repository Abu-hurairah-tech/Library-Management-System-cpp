#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include "admin.h"

using namespace std;

void get_password(string &password, int max_len)
{
    char ch;
    password = "";

    while (password.size() < static_cast<size_t>(max_len - 1))
    {
        ch = getch();
        if (ch == '\r')
        {
            break;
        }
        else if (ch == '\b')
        {
            if (!password.empty())
            {
                password.pop_back();
                cout << "\b \b";
            }
        }
        else if (ch >= 32 && ch <= 126)
        {
            password += ch;
            cout << "*";
        }
    }
    cout << endl;
}

void register_user()
{
    string username, password;
    cout << "\n\n\t\t\t=== Register ===\n\n";
    cout << "Enter new username: ";
    cin >> username;
    cout << "Enter new password: ";
    get_password(password, 12);

    ifstream infile("loginfile.csv");
    if (infile)
    {
        string file_un, file_pass;
        string line;
        getline(infile, line);
        while (getline(infile, line))
        {
            size_t comma = line.find(',');
            if (comma != string::npos)
            {
                file_un = line.substr(0, comma);
                file_pass = line.substr(comma + 1);
                if (file_un == username)
                {
                    cout << "Username already exists. Try a different one.\n";
                    infile.close();
                    return;
                }
            }
        }
        infile.close();
    }

    ofstream outfile("loginfile.csv", ios::app);
    if (!outfile)
    {
        cout << "Error opening file.\n";
        return;
    }

    infile.open("loginfile.csv", ios::in);
    infile.seekg(0, ios::end);
    if (infile.tellg() == 0)
    {
        outfile << "UserName,Password\n";
    }
    infile.close();

    outfile << username << "," << password << "\n";
    outfile.close();
    cout << "User registered successfully!\n";
}

int login_user()
{
    string input_un, input_pass;
    string file_un, file_pass;
    int login = 0;
    int attempts = 0;

    while (attempts < 3)
    {
        cout << "\n\n\t\t\t=== Login (Attempt " << attempts + 1 << " of 3) ===\n\n";
        cout << "Enter username: ";
        cin >> input_un;
