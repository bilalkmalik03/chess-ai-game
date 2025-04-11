# 🧠 C++ Chess Game with AI (SFML)

A fully interactive, feature-complete Chess game built with **C++17** and **SFML**. Includes full rules of chess, a graphical UI, move logging, pawn promotion, and an AI opponent powered by **Minimax with Alpha-Beta Pruning**. Difficulty levels determine how far ahead the bot thinks.

---

## 🧩 Features

- 🎮 **Graphical Interface** — Built with SFML 2.6; supports all standard chess rules and interactions.
- ♟️ **Legal Move Generation** — Handles castling, en passant, promotion, check, checkmate, and stalemate.
- 🧠 **AI Opponent** — Uses Minimax + Alpha-Beta Pruning; depth increases with selected difficulty.
- ↔️ **Move Log** — Real-time move list on the right-hand panel using chess notation (e.g., `e2->e4`).
- 👑 **Promotion Popup** — When a pawn reaches the 8th rank, select a piece via an in-game modal.

---

## 🖥️ Tech Stack

- **Language:** C++17  
- **Graphics Library:** SFML (Simple and Fast Multimedia Library)  
- **Build System:** Visual Studio Code `tasks.json` or manual `g++` build

---

## 🚀 Getting Started

### ✅ Prerequisites

- A C++17-compatible compiler (e.g., g++ 11+, MinGW, MSVC)
- [SFML 2.6.2 or newer](https://www.sfml-dev.org/download.php)
- Visual Studio Code (recommended)

> 💡 Ensure SFML `.dll` files are in the same directory as your executable or accessible via your system PATH.

---

### ⚙️ Building the Project

#### ✔️ Option 1: Using VS Code Tasks

1. Open the folder in VS Code.
2. Press `Ctrl + Shift + B` to run the default build task.
3. The compiled `ChessGame.exe` will be output to the `build/` folder.

> Make sure your `tasks.json` file includes correct compiler and SFML paths.

#### ✔️ Option 2: Manual Compilation

```bash
g++ -std=c++17 -g main.cpp Board.cpp Piece.cpp Bot.cpp ^
-I"C:/Path/To/SFML/include" -L"C:/Path/To/SFML/lib" ^
-lsfml-graphics -lsfml-window -lsfml-system -o build/ChessGame.exe

** Running the Game **
cd build
ChessGame.exe


** Gameplay Instructions **
- Startup Menu: Select your side (White or Black) and difficulty level (Easy, Medium, Hard).
- Move Pieces: Click to select a piece and then click a highlighted square to move it.
- AI: The bot automatically plays its move when it’s its turn.
- Promotion: A popup appears to let you choose a new piece when a pawn reaches the back rank.
- Move Log: Displayed on the right side using chess notation (e.g., e2->e4).

** Ai Logic **
The bot uses a depth-limited Minimax algorithm with Alpha-Beta Pruning:

| Difficulty | Minimax Depth |
|------------|----------------|
| Easy       | 1              |
| Medium     | 2              |
| Hard       | 3              |

Currently the evaluation is based on material value.
You can extend it with positional heuristics like piece-square tables, king safety, and mobility.

** Project Structure

ChessGame/
├── assets/               # Piece textures and fonts
├── Board.hpp / .cpp      # Board logic and move execution
├── Piece.hpp / .cpp      # Piece classes and move generation
├── Bot.hpp / .cpp        # Minimax AI logic and evaluation
├── main.cpp              # Game loop and GUI
├── tasks.json            # VS Code build task
├── build/                # Compiled executable output
└── README.md             # Project documentation


** Future Improvements **
- [ ] Add full undo/redo functionality  
- [ ] Save/load games to file  
- [ ] Smarter AI evaluation (position, king safety, pawn structure)  
- [ ] Animated piece movement  
- [ ] Sound effects for moves/captures  
- [ ] LAN or local multiplayer  
- [ ] Export to PGN format  


** License **
This project is open-source and free to use for educational or personal projects.
You may modify and redistribute it with attribution.


** Acknowledgements **
- SFML Library: https://www.sfml-dev.org/
- Open-source chess textures
- Stack Overflow & open-source communities for algorithm references






