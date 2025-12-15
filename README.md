# BFI — Brainfuck Interpreter (C++)

`bfi` is a **Brainfuck interpreter** written in C++, with a small preprocessing
step that groups repeated instructions to improve execution speed.

The goal of this project is to be simple, readable, and faithful to the
classic Brainfuck language semantics.

---

## Features

- Full support for the 8 Brainfuck instructions:
```

+ - < > , . [ ]

````
- Dynamic memory tape with **8-bit cells (uint8_t)**
- Memory pointer starts at cell 0
- Instruction grouping (`+++`, `>>>`, etc.)
- Preprocessed loops with direct jumps
- Input/output via **stdin / stdout**
- Ignores any non-Brainfuck characters

---

## Execution Model

- Each memory cell is an **unsigned 8-bit integer** (`0–255`)
- Increment and decrement operations wrap around automatically
- The tape grows dynamically to the right
- Moving the pointer left past cell 0 results in an error

---

## Compilation

Requires a compiler with **C++23** support (e.g. `g++` ≥ 13).

```bash
g++ -std=c++23 -O2 -o bfi bfi.cpp
````

---

## Usage

```bash
./bfi file.bf
```

Example:

```bash
./bfi hello.bf
```

---

##  Input (`,`)

* The `,` instruction reads **one byte from standard input**
* If `EOF` is reached, the current cell is set to `0`
* To finish manual input:

  * Linux / macOS: **Ctrl + D**
  * Windows: **Ctrl + Z + Enter**

---

## Output (`.`)

* The `.` instruction outputs the current cell as an ASCII character
* Repeated instructions (`...`) output multiple characters

---

## Loops (`[` and `]`)

* Loops are resolved during parsing
* `[` jumps past the matching `]` if the current cell is zero
* `]` jumps back after the matching `[` if the current cell is non-zero
* Unmatched brackets result in an error

---

## Handled Errors

* File not found
* `]` without a matching `[`
* Memory underflow (pointer moves left of cell 0)

---

## Example

### `hello.bf`

```brainfuck
++++++++++[>+++++++>++++++++++>+++>+<<<<-]
>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.
```

### Run

```bash
./bfi hello.bf
```

### Output

```
Hello World!
```

---

## Internal Structure

* **Lexer**: filters valid Brainfuck characters
* **Parser**: groups repeated instructions
* **Executor**: runs compacted operations
* Loops use absolute instruction indices for fast jumps

---

## License

Educational / experimental project.
Feel free to study, modify, and extend it.

---