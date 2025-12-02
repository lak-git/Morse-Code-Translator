# Morse Code Translator

## Overview

Morse Code Translator is a small, portable C-based Morse code codec that implements a binary tree representation of the international Morse alphabet and digits. The project uses a binary tree where each left edge represents a dot (.) and each right edge represents a dash (-). Encoding and decoding are implemented using classic tree traversal algorithms: pre-order depth-first traversal (DFS) for building and printing the dictionary, and breadth-first traversal (BFS) when helpful for level-order inspection.

The codebase focuses on clear, self-contained C code with minimal dependencies so it can be compiled on most Unix-like systems using GCC or Clang.

---

## Features

- Buildable with GNU Make using GCC or Clang
- Binary tree-backed Morse alphabet and digits (A–Z, 0–9)
- Encode alphabetical text to Morse code
- Decode Morse code (letters separated by spaces, words separated by `/`) to human-readable text
- Print a human-readable Morse code dictionary (tree traversal)
- Small, dependency-free C implementation with focus on readability and correctness

---

## Installation & Setup

Requirements:

- A C compiler supporting C17 (GCC or Clang recommended)
- make

Build steps (from the repository root):

```bash
# Build using the default compiler (CC can be changed)
make

# Or force a specific compiler (GCC or Clang)
make gcc   # Force GCC
make clang # Force Clang
```

The build produces the executable at `build/MorseCodeTranslator`.

---

## Usage

The translator can be used interactively or by passing a file containing a Morse code message as the first argument.

Interactive modes (run without arguments):

1) Morse code -> Alphabetical
   - Enter a Morse message where letters are separated by spaces and words are separated by `/`.
     Example: `.... . .-.. .-.. --- / .-- --- .-. .-.. -..` → "HELLO WORLD"

2) Alphabetical -> Morse code
   - Enter plain text with letters and spaces. Letters will be converted to the corresponding Morse code sequences.
     Example input: `HELLO 123` → output: `.... . .-.. .-.. --- / .---- ..--- ...--`

3) Print Dictionary
   - Prints the Morse code dictionary built from the tree (useful for debugging or inspection).

Command-line (file) mode:

If you pass a filename as the first argument, the program will read the file contents and attempt to decode it as Morse code (Morse -> Alphabetical). The file should contain letters separated by spaces and words separated by `/`.

Example runs:

```bash
# Interactive
./build/MorseCodeTranslator

# Decode a file containing morse code
./build/MorseCodeTranslator path/to/morse_message.txt
```

Notes on input format:

- Morse letters are separated by spaces. For example `.- -... -.-.` corresponds to "ABC".
- Words are separated by a forward slash `/` (space around the slash is optional depending on input).
- The program validates Morse input and will report errors for invalid characters.

---

## License

This project is licensed under the MIT License. See the `LICENSE` file for full text.
