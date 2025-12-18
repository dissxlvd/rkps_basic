@echo off
echo Building Four in a Row...

REM Native build with MinGW (if installed)
if exist "C:\MinGW\bin\gcc.exe" (
    echo 1. Building native version...
    C:\MinGW\bin\gcc -o four_in_a_row.exe four_in_a_row.c
)

echo Done!