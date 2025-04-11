#pragma once
#include "Piece.hpp"
#include <stack>
#include <vector>
#include <string>
using namespace std;

class Board {
public:
    Board();
    Board(const Board& other);
    Board& operator=(const Board& other);
    ~Board();

    void setupBoard();
    Piece* getPiece(int row, int col) const;
    void setPiece(int row, int col, Piece* piece);
    void movePiece(int fromRow, int fromCol, int toRow, int toCol);
    void undoMove();

    bool isWhiteTurn() const { return whiteTurn; }
    void toggleTurn() { whiteTurn = !whiteTurn; }

    bool isInCheck(bool white) const;
    bool hasLegalMoves(bool white);
    bool isCheckmate(bool white);
    bool isStalemate(bool white);

    bool hasKingMoved(bool white) const;
    bool hasRookMoved(bool white, bool kingSide) const;

    void setEnPassantTarget(pair<int, int> target);
    pair<int, int> getEnPassantTarget() const;

    vector<string> moveLog;

private:
    Piece* board[8][8];
    bool whiteTurn = true;

    bool whiteKingMoved = false;
    bool blackKingMoved = false;
    bool whiteRookMoved[2] = { false, false };
    bool blackRookMoved[2] = { false, false };

    pair<int, int> enPassantTarget = { -1, -1 };

    struct Move {
        int fromRow, fromCol;
        int toRow, toCol;
        Piece* movedPiece;
        Piece* capturedPiece;
        bool wasWhiteTurn;
        pair<int, int> prevEnPassant;
    };

    stack<Move> moveHistory;
};
