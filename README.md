# 🏁 BitBoard Checkers Game
**Course:** CS 3503 – Computer Organization  
**Author:** Nyasha Demean Muzerengi  
**Project:** BitBoard Checkers (C Implementation)

---

## 🎯 Overview
This project implements a **console-based Checkers game** entirely using **bitboard representation (64-bit integers)**.  
It demonstrates bit manipulation, control flow, and structured programming.

#### HOW TO COMPILE
gcc main.c board.c moves.c save_load.c -o checkers
./checkers


---

## 🧩 Features
- 🧱 Bitboard-based board representation (using `uint64_t`)
- 🎮 Two-player gameplay (Red vs. Black)
- ♟ Move validation and captures
- 🔁 Multi-jump capture chains
- 👑 King promotion
- 🏁 Win and draw detection
- 💾 Save and load system

---

## ⚙️ Compilation
Use the included **Makefile**:

```bash
make

