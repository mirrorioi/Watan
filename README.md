## Features

- **4-player turn-based game** (Blue, Red, Orange, Yellow)
- **Hex-tile board** with 19 tiles and fixed layout
- **Resources**: `CAFFEINE`, `LAB`, `LECTURE`, `STUDY`, `TUTORIAL`, `NETFLIX` (produces none)
- **Dice system** per player:
  - `fair` dice (random 2–12)
  - `load`ed dice (user inputs 2–12 with validation)
- **Build system**
  - Complete criteria (Assignment → Midterm → Exam)
  - Achieve goals (edges) with adjacency rules
- **Trading** between players
- **Geese** on rolling 7: move geese, forced discards, steal from eligible players
- **Save/Load**
  - Load from a save file
  - Save current state to a file
  - EOF handling: auto-save to `backup.sv`

---

## Getting Started

### Requirements
- A C++ compiler (recommended: **clang++** on macOS, **g++** also works)
- A Unix-like terminal (macOS Terminal is fine)

### Build (example)
If your project has a `Makefile`:
````bash
make
