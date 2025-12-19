#!/bin/bash

echo "Building Four in a Row..."

# Native build
echo "1. Building native version..."
gcc -o bin/Four_In_A_Row ./src/game.c -lm

echo "Done!"