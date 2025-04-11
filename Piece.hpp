#pragma once
#include <vector>
#include <utility>

// Forward declaration of Board to avoid circular dependency.
class Board;

class Piece {
public:
    Piece(bool white) : white(white) {}
    virtual ~Piece() {}

    bool isWhite() const { return white; }
    virtual char getSymbol() const = 0;
    virtual std::vector<std::pair<int, int>> getLegalMoves(int row, int col, const Board& board) const = 0;
    virtual Piece* clone() const = 0;  // Added for deep copying.
protected:
    bool white;
};

class Pawn : public Piece {
public:
    Pawn(bool white) : Piece(white) {}
    char getSymbol() const override { return white ? 'P' : 'p'; }
    std::vector<std::pair<int, int>> getLegalMoves(int row, int col, const Board& board) const override;
    Piece* clone() const override { return new Pawn(*this); }
};

class Rook : public Piece {
public:
    Rook(bool white) : Piece(white) {}
    char getSymbol() const override { return white ? 'R' : 'r'; }
    std::vector<std::pair<int, int>> getLegalMoves(int row, int col, const Board& board) const override;
    Piece* clone() const override { return new Rook(*this); }
};

class Knight : public Piece {
public:
    Knight(bool white) : Piece(white) {}
    char getSymbol() const override { return white ? 'N' : 'n'; }
    std::vector<std::pair<int, int>> getLegalMoves(int row, int col, const Board& board) const override;
    Piece* clone() const override { return new Knight(*this); }
};

class Bishop : public Piece {
public:
    Bishop(bool white) : Piece(white) {}
    char getSymbol() const override { return white ? 'B' : 'b'; }
    std::vector<std::pair<int, int>> getLegalMoves(int row, int col, const Board& board) const override;
    Piece* clone() const override { return new Bishop(*this); }
};

class Queen : public Piece {
public:
    Queen(bool white) : Piece(white) {}
    char getSymbol() const override { return white ? 'Q' : 'q'; }
    std::vector<std::pair<int, int>> getLegalMoves(int row, int col, const Board& board) const override;
    Piece* clone() const override { return new Queen(*this); }
};

class King : public Piece {
public:
    King(bool white) : Piece(white) {}
    char getSymbol() const override { return white ? 'K' : 'k'; }
    std::vector<std::pair<int, int>> getLegalMoves(int row, int col, const Board& board) const override;
    Piece* clone() const override { return new King(*this); }
};
