# C-Minus-Compiler
C- Compiler proposed in the book "Compiler Construction" by Kenneth C. Louden.

Initial base code sourced from 
[Tiny Compiler](http://www.cs.sjsu.edu/faculty/louden/cmptext/ "Tiny Compiler Exemple").

## Modules
* Scanner
* Parser
* Symbol Table
* Semantic Analyzer
* Code Generator

## Requirements
* MAKE
* GCC
* FLEX
* BISON

## How to Use
```
make
.\compiler CminusCode.cm
```

## Instructions
|    |            |                                  |        | 25     -     21 | 20     -     16 | 15     -     11 | 10     -       0  |
|----|:----------:|----------------------------------|:------:|:---------------:|:---------------:|:---------------:|:-----------------:|
|    |            |                                  | 6 bits |      5 bits     |      5 bits     |      5 bits     |      11 bits      |
|    |**Operations**|                                | **OP** |                 |                 |                 |                   |
|----|:----------:|----------------------------------|:------:|:---------------:|:---------------:|:---------------:|:-----------------:|
|  0 |     NOP    | No operation                     | 000000 |        0        |                 |                 |                   |
|  1 |     ADD    | rd = rs + rt                     | 000001 |        rs       |        rt       |        rd       |         0         |
|  2 |    ADDi    | rt = rs + im16                   | 000010 |        rs       |        rt       |   Immediate16   |                   |
|  3 |     SUB    | rd = rs - rt                     | 000011 |        rs       |        rt       |        rd       |         0         |
|  4 |    SUBi    | rt = rs - im16                   | 000100 |        rs       |        rt       |   Immediate16   |                   |
|  5 |    MULT    | rd = rs * rt                     | 000101 |        rs       |        rt       |        rd       |         0         |
|  6 |    MULTi   | rt = rs * imm16                  | 000110 |        rs       |        rt       |   Immediate16   |                   |
|  7 |     DIV    | rd = rs / rt                     | 000111 |        rs       |        rt       |        rd       |         0         |
|  8 |    DIVi    | rt = rs / imm16                  | 001000 |        rs       |        rt       |   Immediate16   |                   |
|  9 |     NOR    | rd = ~(rs \| rt)                 | 001001 |        rs       |        rt       |        rd       |         0         |
| 10 |     AND    | rd = rs & rt                     | 001010 |        rs       |        rt       |        rd       |         0         |
| 11 |    ANDi    | rt = rs & im16                   | 001011 |        rs       |        rt       |   Immediate16   |                   |
| 12 |     OR     | rd = rs \| rt                    | 001100 |        rs       |        rt       |        rd       |         0         |
| 13 |     Ori    | rt = rs \| im16                  | 001101 |        rs       |        rt       |   Immediate16   |                   |
| 14 |     SL     | rt = rs<<shift16                 | 001110 |        rs       |        rt       |     Shift16     |                   |
| 15 |     SR     | rt = rs>>shift16                 | 001111 |        rs       |        rt       |     Shift16     |                   |
| 16 |     SLT    | rd = (rs<rt)? 1 : 0              | 010000 |        rs       |        rt       |        rd       |         0         |
| 17 |    SLTi    | rt = (rs<imm16)? 1 : 0           | 010001 |        rs       |        rt       |   Immediate16   |                   |
| 18 |     BEQ    | IF rs = rt, PC = PC + offset16   | 010010 |        rs       |        rt       |     offset16    |                   |
| 19 |     BNE    | IF rs ≠ rt, PC = PC +   offset16 | 010011 |        rs       |        rt       |     offset16    |                   |
| 20 |     JR     | PC = rs                          | 010100 |        rs       |        0        |        0        |                   |
| 21 |      J     | PC :: Addr26                     | 010101 |                                Addr26                                  ||
| 22 |     JAL    | $ra = PC + 1, PC :: Addr26       | 010110 |                                Addr26                                  ||
| 23 |     LW     | rt = mem[base + offset]          | 010111 |       base      |        rt       |     offset16    |                   |
| 24 |     SW     | mem[base + offset] =  rt         | 011000 |       base      |        rt       |     offset16    |                   |
| 25 |     IN     | rs = switchs                     | 011001 |        rs       |        0        |        0        |         0         |
| 26 |     OUT    | display = rs                     | 011010 |        rs       |        0        |        0        |         0         |
| 27 |     HLT    | Halt                             | 011011 |                                   0                                     |
