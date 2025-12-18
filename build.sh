#!/bin/bash

echo "Building Four in a Row..."

# Native build
echo "1. Building native version..."
gcc -o bin/four_in_a_row four_in_a_row.c -lm

# Web build (if Emscripten is installed)
if command -v emcc &> /dev/null; then
    echo "2. Building WebAssembly version..."
    mkdir -p web
    emcc four_in_a_row.c -o web/four_in_a_row.html -s WASM=1
fi

echo "Done!"