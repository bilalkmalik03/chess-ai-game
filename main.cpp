#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Board.hpp"
#include "Bot.hpp"
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>

using namespace std;
using namespace sf;

const int TILE_SIZE = 80;
const int BOARD_SIZE = TILE_SIZE * 8;
const int WINDOW_WIDTH = BOARD_SIZE + 160;

// Converts board coordinates to algebraic notation, e.g. (6,3) -> "d2".
string toNotation(int row, int col) {
    char file = 'a' + col;
    char rank = '8' - row;
    return string(1, file) + string(1, rank);
}

//------------------------------------------------------------------------------
// Promotion Popup Function
// Displays a modal overlay prompting the user to select a promotion piece.
void handlePromotion(RenderWindow &window, Board &board, const Font &font, int promoRow, int promoCol, bool promoWhite) {
    RectangleShape overlay(Vector2f(window.getSize().x, window.getSize().y));
    overlay.setFillColor(Color(0, 0, 0, 150));

    vector<string> options = { "Queen", "Rook", "Bishop", "Knight" };
    vector<RectangleShape> buttons;
    vector<Text> buttonTexts;

    float buttonWidth = 150.f, buttonHeight = 50.f;
    float spacing = 20.f;
    float totalWidth = options.size() * buttonWidth + (options.size() - 1) * spacing;
    float startX = (window.getSize().x - totalWidth) / 2;
    float yPos = (window.getSize().y - buttonHeight) / 2;

    for (size_t i = 0; i < options.size(); ++i) {
        RectangleShape btn(Vector2f(buttonWidth, buttonHeight));
        btn.setFillColor(Color(200, 200, 200));
        btn.setPosition(startX + i * (buttonWidth + spacing), yPos);
        buttons.push_back(btn);

        Text btnText;
        btnText.setFont(font);
        btnText.setString(options[i]);
        btnText.setCharacterSize(24);
        btnText.setFillColor(Color::Black);
        FloatRect textRect = btnText.getLocalBounds();
        btnText.setOrigin(textRect.left + textRect.width / 2, textRect.top + textRect.height / 2);
        btnText.setPosition(btn.getPosition().x + buttonWidth / 2, btn.getPosition().y + buttonHeight / 2);
        buttonTexts.push_back(btnText);
    }

    bool promoChosen = false;
    char promotionChoice = ' ';

    while (!promoChosen && window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                for (size_t i = 0; i < buttons.size(); ++i) {
                    if (buttons[i].getGlobalBounds().contains(mousePos)) {
                        if      (options[i] == "Queen")  promotionChoice = promoWhite ? 'Q' : 'q';
                        else if (options[i] == "Rook")   promotionChoice = promoWhite ? 'R' : 'r';
                        else if (options[i] == "Bishop") promotionChoice = promoWhite ? 'B' : 'b';
                        else if (options[i] == "Knight") promotionChoice = promoWhite ? 'N' : 'n';
                        promoChosen = true;
                        break;
                    }
                }
            }
        }
        window.clear();
        window.draw(overlay);
        for (size_t i = 0; i < buttons.size(); ++i) {
            window.draw(buttons[i]);
            window.draw(buttonTexts[i]);
        }
        window.display();
    }
    
    Piece* promotedPiece = nullptr;
    switch (promotionChoice) {
        case 'Q': case 'q': promotedPiece = new Queen(promoWhite); break;
        case 'R': case 'r': promotedPiece = new Rook(promoWhite); break;
        case 'B': case 'b': promotedPiece = new Bishop(promoWhite); break;
        case 'N': case 'n': promotedPiece = new Knight(promoWhite); break;
    }
    delete board.getPiece(promoRow, promoCol);
    board.setPiece(promoRow, promoCol, promotedPiece);
}

//------------------------------------------------------------------------------
// Main
//------------------------------------------------------------------------------
int main() {
    RenderWindow window(VideoMode(WINDOW_WIDTH, BOARD_SIZE + 50), "Chess Game");

    bool gameReady = false, playerIsWhite = true;
    int botDepth = 1;

    Font font;
    if (!font.loadFromFile("C:/Users/bilal/Chess Game/assets/ariblk.ttf")) {
        cerr << "Error loading font!" << endl;
        return -1;
    }

    // Setup title and side/difficulty options.
    Text title;
    title.setFont(font);
    title.setString("Choose Side & Difficulty");
    title.setCharacterSize(32);
    title.setPosition(Vector2f(BOARD_SIZE / 2 - 190, 40));
    title.setFillColor(Color::Black);

    vector<Text> labels;
    vector<RectangleShape> buttons;
    vector<string> options = {
        "White (Easy)", "White (Medium)", "White (Hard)",
        "Black (Easy)", "Black (Medium)", "Black (Hard)"
    };
    for (int i = 0; i < 6; ++i) {
        RectangleShape btn(Vector2f(280.f, 30.f));
        btn.setFillColor(Color(200, 200, 200));
        btn.setPosition(Vector2f(BOARD_SIZE / 2 - 140, 100 + i * 40));
        buttons.push_back(btn);

        Text label;
        label.setFont(font);
        label.setString(options[i]);
        label.setCharacterSize(20);
        label.setFillColor(Color::Black);
        label.setPosition(Vector2f(btn.getPosition().x + 10, btn.getPosition().y + 2));
        labels.push_back(label);
    }

    // Load piece textures.
    map<char, Texture> textures;
    for (char s : { 'P', 'R', 'N', 'B', 'Q', 'K', 'p', 'r', 'n', 'b', 'q', 'k' }) {
        Texture t;
        string fileName;
        if      (s == 'P') fileName = "C:/Users/bilal/Chess Game/assets/P_white.png.png";
        else if (s == 'R') fileName = "C:/Users/bilal/Chess Game/assets/R_white.png.png";
        else if (s == 'N') fileName = "C:/Users/bilal/Chess Game/assets/N_white.png.png";
        else if (s == 'B') fileName = "C:/Users/bilal/Chess Game/assets/B_white.png.png";
        else if (s == 'Q') fileName = "C:/Users/bilal/Chess Game/assets/Q_white.png.png";
        else if (s == 'K') fileName = "C:/Users/bilal/Chess Game/assets/K_white.png.png";
        else if (s == 'p') fileName = "C:/Users/bilal/Chess Game/assets/p_black.png.png";
        else if (s == 'r') fileName = "C:/Users/bilal/Chess Game/assets/r_black.png.png";
        else if (s == 'n') fileName = "C:/Users/bilal/Chess Game/assets/n_black.png.png";
        else if (s == 'b') fileName = "C:/Users/bilal/Chess Game/assets/b_black.png.png";
        else if (s == 'q') fileName = "C:/Users/bilal/Chess Game/assets/q_black.png.png";
        else if (s == 'k') fileName = "C:/Users/bilal/Chess Game/assets/k_black.png.png";

        if (!t.loadFromFile(fileName)) {
            cerr << "Error loading texture: " << fileName << endl;
            continue;
        }
        textures[s] = t;
    }

    // Difficulty selection loop.
    while (!gameReady && window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::MouseButtonPressed) {
                int x = event.mouseButton.x;
                int y = event.mouseButton.y;
                if (event.mouseButton.button == Mouse::Left) {
                    for (int i = 0; i < 6; ++i) {
                        if (buttons[i].getGlobalBounds().contains(Vector2f(x, y))) {
                            playerIsWhite = (i < 3);
                            botDepth = (i % 3) + 2;
                            gameReady = true;
                        }
                    }
                }
            }
        }
        window.clear(Color(240, 240, 240));
        window.draw(title);
        for (int i = 0; i < 6; ++i) {
            window.draw(buttons[i]);
            window.draw(labels[i]);
        }
        window.display();
    }

    // Initialize chess board.
    Board board;

    // (Optional) Reload textures for gameplay.
    for (char s : { 'P','R','N','B','Q','K','p','r','n','b','q','k' }) {
        Texture t;
        string fileName;
        if      (s == 'P') fileName = "C:/Users/bilal/Chess Game/assets/P_white.png.png";
        else if (s == 'R') fileName = "C:/Users/bilal/Chess Game/assets/R_white.png.png";
        else if (s == 'N') fileName = "C:/Users/bilal/Chess Game/assets/N_white.png.png";
        else if (s == 'B') fileName = "C:/Users/bilal/Chess Game/assets/B_white.png.png";
        else if (s == 'Q') fileName = "C:/Users/bilal/Chess Game/assets/Q_white.png.png";
        else if (s == 'K') fileName = "C:/Users/bilal/Chess Game/assets/K_white.png.png";
        else if (s == 'p') fileName = "C:/Users/bilal/Chess Game/assets/p_black.png.png";
        else if (s == 'r') fileName = "C:/Users/bilal/Chess Game/assets/r_black.png.png";
        else if (s == 'n') fileName = "C:/Users/bilal/Chess Game/assets/n_black.png.png";
        else if (s == 'b') fileName = "C:/Users/bilal/Chess Game/assets/b_black.png.png";
        else if (s == 'q') fileName = "C:/Users/bilal/Chess Game/assets/q_black.png.png";
        else if (s == 'k') fileName = "C:/Users/bilal/Chess Game/assets/k_black.png.png";

        if (!t.loadFromFile(fileName)) {
            cerr << "Error loading texture: " << fileName << endl;
            continue;
        }
        textures[s] = t;
    }

    // Setup the move log panel.
    RectangleShape movePanel(Vector2f(160.f, BOARD_SIZE));
    movePanel.setPosition(Vector2f(BOARD_SIZE, 0));
    movePanel.setFillColor(Color(245, 245, 245));

    // Setup status text.
    Text statusText;
    statusText.setFont(font);
    statusText.setCharacterSize(24);
    statusText.setFillColor(Color::Red);
    statusText.setPosition(10, BOARD_SIZE + 5);

    // Selection and promotion variables.
    bool selected = false;
    int selRow = -1, selCol = -1;
    set<pair<int, int>> legalMoves;
    bool promotionPending = false;
    int promoRow = -1, promoCol = -1;
    bool promoWhite = false;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (promotionPending)
                continue;

            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                int mx = event.mouseButton.x / TILE_SIZE;
                int my = event.mouseButton.y / TILE_SIZE;
                int col = playerIsWhite ? mx : 7 - mx;
                int row = playerIsWhite ? my : 7 - my;
                cerr << "Clicked on row: " << row << ", col: " << col << endl;

                if (selected) {
                    if (legalMoves.count({ row, col })) {
                        board.movePiece(selRow, selCol, row, col);
                        cerr << "Moved piece from (" << selRow << ", " << selCol 
                             << ") to (" << row << ", " << col << ")" << endl;
                        selected = false;
                        legalMoves.clear();

                        // Check for promotion.
                        Piece* movedPiece = board.getPiece(row, col);
                        if (movedPiece && ((movedPiece->getSymbol() == 'P' && row == 0) ||
                                           (movedPiece->getSymbol() == 'p' && row == 7))) {
                            promotionPending = true;
                            promoRow = row;
                            promoCol = col;
                            promoWhite = movedPiece->isWhite();
                        }
                    } else {
                        Piece* piece = board.getPiece(row, col);
                        if (piece && piece->isWhite() == playerIsWhite &&
                            piece->isWhite() == board.isWhiteTurn()) {
                            selRow = row;
                            selCol = col;
                            legalMoves.clear();
                            for (auto& m : piece->getLegalMoves(row, col, board)) {
                                Board temp = board;
                                temp.movePiece(row, col, m.first, m.second);
                                if (!temp.isInCheck(piece->isWhite()))
                                    legalMoves.insert(m);
                            }
                            cerr << "Reselected piece at (" << row << ", " << col << ")" << endl;
                        } else {
                            selected = false;
                            legalMoves.clear();
                        }
                    }
                } else {
                    Piece* piece = board.getPiece(row, col);
                    if (piece) {
                        cerr << "Piece selected: " << piece->getSymbol() << endl;
                        if (piece->isWhite() == playerIsWhite &&
                            piece->isWhite() == board.isWhiteTurn()) {
                            selRow = row;
                            selCol = col;
                            selected = true;
                            legalMoves.clear();
                            for (auto& m : piece->getLegalMoves(row, col, board)) {
                                Board temp = board;
                                temp.movePiece(row, col, m.first, m.second);
                                if (!temp.isInCheck(piece->isWhite()))
                                    legalMoves.insert(m);
                            }
                        } else {
                            cerr << "Invalid piece selected." << endl;
                        }
                    } else {
                        cerr << "No piece at this location!" << endl;
                    }
                }
            }
        }

        if (promotionPending) {
            handlePromotion(window, board, font, promoRow, promoCol, promoWhite);
            promotionPending = false;
        }

        if (!selected && board.isWhiteTurn() != playerIsWhite) {
            this_thread::sleep_for(chrono::milliseconds(500));
            Bot::makeMove(board, botDepth, board.isWhiteTurn());
        }

        string status;
        if (board.isCheckmate(true))
            status = "Checkmate! Black wins!";
        else if (board.isCheckmate(false))
            status = "Checkmate! White wins!";
        else if (board.isInCheck(true))
            status = "White is in check!";
        else if (board.isInCheck(false))
            status = "Black is in check!";
        else if (board.isStalemate(true) || board.isStalemate(false))
            status = "Stalemate!";
        statusText.setString(status);

        // Dynamically compute the maximum number of move pairs (lines) that fit in the move panel.
        int lineHeight = 18; // Reduced line height for smaller text.
        int maxMovePairs = static_cast<int>(movePanel.getSize().y) / lineHeight;
        int totalMoves = static_cast<int>(board.moveLog.size());
        int startIndex = max(0, totalMoves - maxMovePairs * 2);

        string moveListStr;
        for (int i = startIndex; i < totalMoves; i += 2) {
            int moveNumber = i / 2 + 1;
            moveListStr += to_string(moveNumber) + ". " + board.moveLog[i];
            if (i + 1 < totalMoves)
                moveListStr += " " + board.moveLog[i + 1];
            moveListStr += "\n";
        }

        Text moveLogText;
        moveLogText.setFont(font);
        moveLogText.setCharacterSize(14); // Reduced text size.
        moveLogText.setFillColor(Color::Black);
        moveLogText.setString(moveListStr);
        moveLogText.setPosition(BOARD_SIZE + 10, 10);

        window.clear(Color::White);
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                int row = playerIsWhite ? i : 7 - i;
                int col = playerIsWhite ? j : 7 - j;
                RectangleShape tile(Vector2f(TILE_SIZE, TILE_SIZE));
                tile.setPosition(Vector2f(j * TILE_SIZE, i * TILE_SIZE));
                bool light = (row + col) % 2 == 0;
                tile.setFillColor(light ? Color(240, 217, 181) : Color(181, 136, 99));
                if (selected && row == selRow && col == selCol)
                    tile.setFillColor(Color::Yellow);
                if (legalMoves.count({ row, col }))
                    tile.setFillColor(Color(100, 200, 100));
                window.draw(tile);

                Piece* piece = board.getPiece(row, col);
                if (piece) {
                    char symbol = piece->getSymbol();
                    Sprite sprite(textures[symbol]);
                    sprite.setPosition(Vector2f(j * TILE_SIZE, i * TILE_SIZE));
                    sprite.setScale((float)TILE_SIZE / sprite.getTexture()->getSize().x,
                                    (float)TILE_SIZE / sprite.getTexture()->getSize().y);
                    window.draw(sprite);
                }
            }
        }
        window.draw(movePanel);
        window.draw(moveLogText);
        window.draw(statusText);
        window.display();
    }
    return 0;
}
