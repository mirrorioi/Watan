# Students of Watan (Catan-like) — C++ CLI Game

A command-line implementation of **Students of Watan**, a University of Waterloo–themed variant of *Settlers of Catan*. The game supports **4 human players**, turn-based gameplay, resource generation from dice rolls, building/upgrading criteria, achieving goals, trading, geese (robber) mechanics, and saving/loading game state.

> This project is based on a course project specification. Rules and I/O format follow the provided spec as closely as possible.

---

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

### To build
````bash
make
