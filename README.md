# ?? Library DSA Edition

A fully functional **CLI Library Management System** written in **C**, demonstrating File I/O, dynamic memory, and basic data structures.

## Features
- Add / list / search / delete books
- Add / list members
- Issue and return books
- File-backed storage: books.dat, members.dat, issues.dat
- Modular code: db.c, main.c, user.c

## Build & Run (Windows PowerShell)
gcc -std=c11 -Wall -Wextra -Iinclude src\db.c src\user.c src\main.c -o build\library.exe
& ".\build\library.exe"

## Author
Raj Kumar Sah
