Compile with GCC:

---
Windows:
```
gcc game.c -o <exe file>
```
---

---
Linux:
```
gcc game.c -o <out file> -lm
```
---

---
For Web version:

1. Install chocolatey: https://community.chocolatey.org/
2. Install emscripten via chocolatey: https://community.chocolatey.org/packages/emscripten
3. Run build script: ```emcc game.c -o four_in_a_row.html -s WASM=1```
---
