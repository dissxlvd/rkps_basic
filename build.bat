@echo off
echo Building Four in a Row...

REM Native build with MinGW (if installed)
if exist "C:\MinGW\bin\gcc.exe" (
    echo 1. Building native version...
    C:\MinGW\bin\gcc -o bin\Four_In_A_Row.exe src\game.c
)

echo Done!