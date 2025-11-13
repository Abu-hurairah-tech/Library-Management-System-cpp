# Library Management System (C++)

This is a small console-based Library Management System (LMS) written in C++.
It uses plain CSV files for persistent storage and a simple menu-driven console UI.

This README explains the project layout, how to build and run the program on Windows (PowerShell), the CSV formats used by the program, important runtime behaviors, and recent fixes and validations added while maintaining original functionality.

## Repository structure (FinalModule)

Key source files (in `FinalModule/`):

- `final.cpp` — Program entry point and main menu that routes to portals.
- `admin.cpp`, `admin.h` — Admin / login related code.
- `member_portal.cpp`, `member.h` — Member management (add, search, delete, display).
- `books_portal.cpp`, `books.h` — Books management (add, search, delete, display).
- `issue_portal.cpp`, `issue.h` — Book issuing and issue history.
- `return_portal.cpp`, `return.h` — Return portal and fine handling interaction.
- `fine_portal.cpp`, `fine.h` — Fine slip/clearance utilities.

Data files (CSV) used by the program:

- `member.csv` — Members (header: `Name,ID,Department,Session,Contact`)
- `books.csv` — Books (header: `ID,Title,Author`)
- `issue.csv` — Currently issued books
- `history.csv` — Issue history (used by issue portal)
- `fine.csv` — Fines
- `loginfile.csv` — Admin credentials (used by `admin.cpp`)

## Build (Windows, PowerShell)

Prerequisites: MinGW/g++ or other C++ compiler supporting C++11 or later.

Open PowerShell in the `FinalModule` folder and run either:

```
g++ *.cpp -o library.exe
```

or build the final executable explicitly (example):

```
g++ .\final.cpp .\admin.cpp .\books_portal.cpp .\fine_portal.cpp .\issue_portal.cpp .\member_portal.cpp .\return_portal.cpp -o final.exe
```

Then run:

```
.\library.exe
```

or

```
.\final.exe
```

Note: The project was developed and tested on Windows with MinGW g++.

## CSV formats

Members (`member.csv`):

Header: `Name,ID,Department,Session,Contact`

Example row:

```
John Doe,BS123,Computer Science,2019,03123456789
```

Books (`books.csv`):

Header: `ID,Title,Author`

Example row:

```
B001,The C++ Programming Language,Bjarne Stroustrup
```

Issued books, history and fines are stored in their respective CSVs. The code reads and writes these files line-oriented using `ifstream`/`ofstream`.

## Important behavior & validations

- Input validation: menus now validate numeric input robustly (the program checks `cin >> choice` and clears invalid input) to prevent crashes when the user types non-numeric input.
- Member contact validation: when adding a member the contact input is restricted to numeric digits only and must be exactly 11 digits long. The program will repeatedly prompt until a valid 11-digit number is entered.
- File-handling: the program uses CSV files and performs safe write/replace using temporary files (for deletes) to reduce data loss risk.

These validations were implemented to make the application beginner-friendly and to avoid the program entering an invalid state because of malformed console input.

## How to use (quick)

1. Run the application executable.
2. Use the main menu to navigate to:
   - Member Management
   - Books Management
   - Issue/Return/Fine portals
3. Follow on-screen prompts. When entering numeric menu choices, type a number (e.g., `1`) and press Enter.

## Debugging & common issues

- If the program fails to start, ensure you compiled all `.cpp` files together and used the correct path to the executable.
- If CSV files are missing, the program will create new ones with headers when needed.
- If a menu appears to freeze after entering invalid input, make sure you typed a number and pressed Enter; the updated input validation should recover and re-prompt.
