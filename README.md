#  C++ Chess Game with AI (SFML)

A fully interactive, feature-complete Chess game built with **C++17** and **SFML**. Includes full rules of chess, a graphical UI, move logging, pawn promotion, and an AI opponent powered by **Minimax with Alpha-Beta Pruning**. Difficulty levels determine how far ahead the bot thinks.

---

##  Features

-  **Graphical Interface** — Built with SFML 2.6; supports all standard chess rules and interactions.
-  **Legal Move Generation** — Handles castling, en passant, promotion, check, checkmate, and stalemate.
-  **AI Opponent** — Uses Minimax + Alpha-Beta Pruning; depth increases with selected difficulty.
-  **Move Log** — Real-time move list on the right-hand panel using chess notation (e.g., `e2->e4`).
-  **Promotion Popup** — When a pawn reaches the 8th rank, select a piece via an in-game modal.

---
## 📽️ Demo Video

Click to play the demo video: https://youtu.be/fGkHcV6ynM8 

##  Getting Started

###  Prerequisites

- A C++17-compatible compiler (e.g., g++ 11+, MinGW, MSVC)
- [SFML 2.6.2 or newer](https://www.sfml-dev.org/download.php)
- Visual Studio Code (recommended)

>  Ensure SFML `.dll` files are in the same directory as your executable or accessible via your system PATH.

---

###  Building the Project

####  Option 1: Using VS Code Tasks

1. Open the folder in VS Code.
2. Press `Ctrl + Shift + B` to run the default build task.
3. The compiled `ChessGame.exe` will be output to the `build/` folder.

> Make sure your `tasks.json` file includes correct compiler and SFML paths.

####  Option 2: Manual Compilation

Make sure SFML is installed. Then compile using the following command:

```bash
g++ -std=c++17 -g main.cpp Board.cpp Piece.cpp Bot.cpp ^
-I"C:/Path/To/SFML/include" -L"C:/Path/To/SFML/lib" ^
-lsfml-graphics -lsfml-window -lsfml-system -o build/ChessGame.exe
```

>  Replace `"C:/Path/To/SFML"` with your actual SFML install path.

---

##  Running the Game

```bash
cd build
ChessGame.exe
```

---

##  Gameplay Instructions

- **Startup Menu**: Choose your side (White or Black) and difficulty level (Easy, Medium, Hard).
- **Move Pieces**: Click to select a piece, then click a highlighted square to move.
- **AI Turn**: The bot plays automatically after your move.
- **Pawn Promotion**: A popup lets you choose a piece when promoting a pawn.
- **Move Log**: Shown on the right in standard notation (e.g., `e2→e4`).

---

##  AI Logic

The bot uses a **depth-limited Minimax algorithm** with **Alpha-Beta Pruning** for optimal performance.

| Difficulty | Minimax Depth |
|------------|----------------|
| Easy       | 1              |
| Medium     | 2              |
| Hard       | 3              |

Currently, the evaluation function is based on material value only.

>  Future improvements can include:
> - Piece-square tables  
> - King safety  
> - Pawn structure and mobility heuristics

---

##  Project Structure

```
ChessGame/
├── assets/               # Textures and fonts
├── Board.hpp / .cpp      # Game board logic and move validation
├── Piece.hpp / .cpp      # Piece definitions and legal move generation
├── Bot.hpp / .cpp        # Minimax AI with Alpha-Beta pruning
├── main.cpp              # GUI rendering and game loop
├── tasks.json            # VS Code build task configuration
├── build/                # Output executable folder
└── README.md             # Project documentation
```

---

##  Future Improvements

- [ ] Undo/Redo functionality  
- [ ] Save/Load game state  
- [ ] Smarter AI evaluation (position, king safety, structure)  
- [ ] Animated piece movement  
- [ ] Sound effects for moves and captures  
- [ ] LAN / Local multiplayer  
- [ ] PGN export support  

---

##  License

This project is open-source and free to use for **educational** or **personal** purposes.  
You are welcome to modify and redistribute it with proper attribution.

---

##  Acknowledgements

- [SFML Library](https://www.sfml-dev.org/) — Simple and Fast Multimedia Library  
- Open-source chess textures  
- Stack Overflow and open-source communities for algorithmic references





