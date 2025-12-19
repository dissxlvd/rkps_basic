### Compile with GCC:

Windows:
```
gcc game.c -o <exe file>
```

Linux:
```
gcc game.c -o <out file> -lm
```

### Build with ```make```
- Windows build: run ```make windows```
- Linux build: run ```make linux```
Web version:
- Web build: run ```make wasm``` [!]
or
- Navigate in src and run index.html

[!] For Web version:

1. Install chocolatey: https://community.chocolatey.org/
2. Install emscripten via chocolatey: https://community.chocolatey.org/packages/emscripten
3. Run build script: ```emcc game.c -o four_in_a_row.html -s WASM=1``` or run make wasm build script from above
