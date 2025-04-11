#include "Board.hpp"
#include "Piece.hpp"
#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

//-------------------------------
// Constructors, Copy, and Destructor
//-------------------------------

Board::Board() {
    setupBoard();
}

// Deep copy constructor: clones each piece.
Board::Board(const Board& other) {
    whiteTurn = other.whiteTurn;
    whiteKingMoved = other.whiteKingMoved;
    blackKingMoved = other.blackKingMoved;
    whiteRookMoved[0] = other.whiteRookMoved[0];
    whiteRookMoved[1] = other.whiteRookMoved[1];
    blackRookMoved[0] = other.blackRookMoved[0];
    blackRookMoved[1] = other.blackRookMoved[1];
    enPassantTarget = other.enPassantTarget;
    moveLog = other.moveLog;
    while (!moveHistory.empty()) moveHistory.pop();

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (other.board[r][c])
                board[r][c] = other.board[r][c]->clone();
            else
                board[r][c] = nullptr;
        }
    }
}

// Copy assignment operator: cleans and deep-copies.
Board& Board::operator=(const Board& other) {
    if (this != &other) {
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                delete board[r][c];
            }
        }
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                if (other.board[r][c])
                    board[r][c] = other.board[r][c]->clone();
                else
                    board[r][c] = nullptr;
            }
        }
        whiteTurn = other.whiteTurn;
        whiteKingMoved = other.whiteKingMoved;
        blackKingMoved = other.blackKingMoved;
        whiteRookMoved[0] = other.whiteRookMoved[0];
        whiteRookMoved[1] = other.whiteRookMoved[1];
        blackRookMoved[0] = other.blackRookMoved[0];
        blackRookMoved[1] = other.blackRookMoved[1];
        enPassantTarget = other.enPassantTarget;
        moveLog = other.moveLog;
        while (!moveHistory.empty()) moveHistory.pop();
    }
    return *this;
}

Board::~Board() {
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            delete board[r][c];
}

//-------------------------------
// Board Setup and Accessors
//-------------------------------

void Board::setupBoard() {
    // Initialize all cells to nullptr.
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            board[r][c] = nullptr;

    // Place Black pieces on rows 0 and 1.
    board[0][0] = new Rook(false); board[0][1] = new Knight(false); board[0][2] = new Bishop(false);
    board[0][3] = new Queen(false); board[0][4] = new King(false); board[0][5] = new Bishop(false);
    board[0][6] = new Knight(false); board[0][7] = new Rook(false);
    for (int c = 0; c < 8; ++c)
        board[1][c] = new Pawn(false);

    // Place White pieces on rows 6 and 7.
    for (int c = 0; c < 8; ++c)
        board[6][c] = new Pawn(true);
    board[7][0] = new Rook(true); board[7][1] = new Knight(true); board[7][2] = new Bishop(true);
    board[7][3] = new Queen(true); board[7][4] = new King(true); board[7][5] = new Bishop(true);
    board[7][6] = new Knight(true); board[7][7] = new Rook(true);

    whiteTurn = true;
    whiteKingMoved = blackKingMoved = false;
    whiteRookMoved[0] = whiteRookMoved[1] = false;
    blackRookMoved[0] = blackRookMoved[1] = false;
    enPassantTarget = { -1, -1 };
    moveLog.clear();
    while (!moveHistory.empty()) moveHistory.pop();
}

Piece* Board::getPiece(int row, int col) const {
    return board[row][col];
}

void Board::setPiece(int row, int col, Piece* piece) {
    board[row][col] = piece;
}

//-------------------------------
// Moving Pieces and Undoing Moves
//-------------------------------

void Board::movePiece(int fromRow, int fromCol, int toRow, int toCol) {
    Piece* piece = board[fromRow][fromCol];
    if (!piece || piece->isWhite() != whiteTurn) return;

    Move move;
    move.fromRow = fromRow;
    move.fromCol = fromCol;
    move.toRow = toRow;
    move.toCol = toCol;
    move.movedPiece = piece;
    move.capturedPiece = board[toRow][toCol];
    move.wasWhiteTurn = whiteTurn;
    move.prevEnPassant = enPassantTarget;

    // En passant: execute only if destination square is empty.
    if ((piece->getSymbol() == 'P' || piece->getSymbol() == 'p') &&
        board[toRow][toCol] == nullptr &&
        enPassantTarget == make_pair(toRow, toCol)) {
        int capRow = whiteTurn ? toRow + 1 : toRow - 1;
        move.capturedPiece = board[capRow][toCol];
        delete board[capRow][toCol];
        board[capRow][toCol] = nullptr;
    }

    // Handle castling.
    if ((piece->getSymbol() == 'K' || piece->getSymbol() == 'k') && abs(toCol - fromCol) == 2) {
        int row = fromRow;
        if (toCol == 6) {
            board[row][5] = board[row][7];
            board[row][7] = nullptr;
        } else if (toCol == 2) {
            board[row][3] = board[row][0];
            board[row][0] = nullptr;
        }
    }

    // Update move flags.
    if (piece->getSymbol() == 'K') whiteKingMoved = true;
    if (piece->getSymbol() == 'k') blackKingMoved = true;
    if (piece->getSymbol() == 'R') {
        if (fromCol == 0) whiteRookMoved[0] = true;
        if (fromCol == 7) whiteRookMoved[1] = true;
    }
    if (piece->getSymbol() == 'r') {
        if (fromCol == 0) blackRookMoved[0] = true;
        if (fromCol == 7) blackRookMoved[1] = true;
    }

    // Update en passant target.
    if (piece->getSymbol() == 'P' && fromRow == 6 && toRow == 4)
        enPassantTarget = { 5, toCol };
    else if (piece->getSymbol() == 'p' && fromRow == 1 && toRow == 3)
        enPassantTarget = { 2, toCol };
    else
        enPassantTarget = { -1, -1 };

    // Delete any piece at the destination (normal capture) and complete the move.
    delete board[toRow][toCol];
    board[toRow][toCol] = piece;
    board[fromRow][fromCol] = nullptr;

    moveHistory.push(move);

    // Log the move using ASCII arrow "->" instead of a Unicode arrow.
    char file1 = 'a' + fromCol, file2 = 'a' + toCol;
    char rank1 = '8' - fromRow, rank2 = '8' - toRow;
    moveLog.push_back(string() + file1 + rank1 + "->" + file2 + rank2);

    toggleTurn();
}

void Board::undoMove() {
    if (moveHistory.empty()) return;
    Move m = moveHistory.top(); moveHistory.pop();

    delete board[m.toRow][m.toCol];
    board[m.fromRow][m.fromCol] = m.movedPiece;
    board[m.toRow][m.toCol] = m.capturedPiece;
    whiteTurn = m.wasWhiteTurn;
    enPassantTarget = m.prevEnPassant;
    if (!moveLog.empty()) moveLog.pop_back();
}

//-------------------------------
// Utility Functions
//-------------------------------

bool Board::hasKingMoved(bool white) const {
    return white ? whiteKingMoved : blackKingMoved;
}

bool Board::hasRookMoved(bool white, bool kingSide) const {
    return white ? whiteRookMoved[kingSide] : blackRookMoved[kingSide];
}

pair<int, int> Board::getEnPassantTarget() const {
    return enPassantTarget;
}

void Board::setEnPassantTarget(pair<int, int> target) {
    enPassantTarget = target;
}

//-------------------------------
// Check, Checkmate, and Stalemate Functions
//-------------------------------

// Helper to find the king's position.
pair<int, int> findKing(const Board& board, bool white) {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = board.getPiece(r, c);
            if (p && p->getSymbol() == (white ? 'K' : 'k'))
                return { r, c };
        }
    }
    return { -1, -1 };
}

bool Board::isInCheck(bool white) const {
    pair<int, int> kingPos = findKing(*this, white);
    if (kingPos.first == -1)
        return false; // Should not happen in a valid game.

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = board[r][c];
            if (p && p->isWhite() != white) {
                vector<pair<int, int>> moves = p->getLegalMoves(r, c, *this);
                for (auto& m : moves) {
                    if (m.first == kingPos.first && m.second == kingPos.second)
                        return true;
                }
            }
        }
    }
    return false;
}

bool Board::isCheckmate(bool white) {
    if (!isInCheck(white))
        return false;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = getPiece(r, c);
            if (p && p->isWhite() == white) {
                vector<pair<int, int>> moves = p->getLegalMoves(r, c, *this);
                for (auto& m : moves) {
                    Board temp = *this;
                    temp.movePiece(r, c, m.first, m.second);
                    if (!temp.isInCheck(white))
                        return false;
                }
            }
        }
    }
    return true;
}

bool Board::isStalemate(bool white) {
    if (isInCheck(white))
        return false;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = getPiece(r, c);
            if (p && p->isWhite() == white) {
                vector<pair<int, int>> moves = p->getLegalMoves(r, c, *this);
                for (auto& m : moves) {
                    Board temp = *this;
                    temp.movePiece(r, c, m.first, m.second);
                    if (!temp.isInCheck(white))
                        return false;
                }
            }
        }
    }
    return true;
}

bool Board::hasLegalMoves(bool white) {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = getPiece(r, c);
            if (p && p->isWhite() == white) {
                vector<pair<int, int>> moves = p->getLegalMoves(r, c, *this);
                for (auto& m : moves) {
                    Board temp = *this;
                    temp.movePiece(r, c, m.first, m.second);
                    if (!temp.isInCheck(white))
                        return true;
                }
            }
        }
    }
    return false;
}
