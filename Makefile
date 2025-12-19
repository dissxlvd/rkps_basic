CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = FourInARow

# Default target (Linux)
all: linux

# Linux build
linux: $(TARGET)-linux

$(TARGET)-linux: src\game.c
	$(CC) $(CFLAGS) -o $(TARGET)-linux src\game.c

# Windows build (requires mingw-w64)
windows: $(TARGET)-windows.exe

$(TARGET)-windows.exe: src\game.c
	x86_64-w64-mingw32-gcc $(CFLAGS) -o $(TARGET)-windows.exe src\game.c

# WebAssembly build (requires emscripten)
wasm: $(TARGET).html

$(TARGET).html: src\game.c
	emcc src\game.c -O2 -s WASM=1 -o $(TARGET).html \
		--shell-file src\webasm.html \
		-s EXIT_RUNTIME=1 \
		-s NO_EXIT_RUNTIME=0 \
		-s ALLOW_MEMORY_GROWTH=1

# Clean up
clean:
	rm -f $(TARGET)-linux $(TARGET)-windows.exe \
	      $(TARGET).html $(TARGET).js $(TARGET).wasm

# Phony targets
.PHONY: all linux windows wasm clean