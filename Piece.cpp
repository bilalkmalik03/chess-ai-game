#include "Piece.hpp"
#include "Board.hpp"
#include <cmath>
using namespace std;

vector<pair<int, int>> Pawn::getLegalMoves(int row, int col, const Board& board) const {
    vector<pair<int, int>> moves;
    int dir = white ? -1 : 1;
    int startRow = white ? 6 : 1;

    // Move forward.
    if (!board.getPiece(row + dir, col))
        moves.emplace_back(row + dir, col);

    // Double move from starting position.
    if (row == startRow && !board.getPiece(row + dir, col) && !board.getPiece(row + 2 * dir, col))
        moves.emplace_back(row + 2 * dir, col);

    // Captures.
    for (int dc : {-1, 1}) {
        int c = col + dc;
        if (c >= 0 && c < 8) {
            auto target = board.getPiece(row + dir, c);
            if (target && target->isWhite() != white)
                moves.emplace_back(row + dir, c);
        }
    }

    // En passant.
    auto ep = board.getEnPassantTarget();
    if (ep.first == row + dir && abs(ep.second - col) == 1)
        moves.emplace_back(ep);

    return moves;
}

vector<pair<int, int>> Rook::getLegalMoves(int row, int col, const Board& board) const {
    vector<pair<int, int>> moves;
    int dirs[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
    for (auto& d : dirs) {
        int r = row + d[0], c = col + d[1];
        while (r >= 0 && r < 8 && c >= 0 && c < 8) {
            auto piece = board.getPiece(r, c);
            if (!piece) {
                moves.emplace_back(r, c);
            } else {
                if (piece->isWhite() != white)
                    moves.emplace_back(r, c);
                break;
            }
            r += d[0];
            c += d[1];
        }
    }
    return moves;
}

vector<pair<int, int>> Knight::getLegalMoves(int row, int col, const Board& board) const {
    vector<pair<int, int>> moves;
    int jumps[8][2] = { {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1} };
    for (auto& j : jumps) {
        int r = row + j[0], c = col + j[1];
        if (r >= 0 && r < 8 && c >= 0 && c < 8) {
            auto piece = board.getPiece(r, c);
            if (!piece || piece->isWhite() != white)
                moves.emplace_back(r, c);
        }
    }
    return moves;
}

vector<pair<int, int>> Bishop::getLegalMoves(int row, int col, const Board& board) const {
    vector<pair<int, int>> moves;
    int dirs[4][2] = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };
    for (auto& d : dirs) {
        int r = row + d[0], c = col + d[1];
        while (r >= 0 && r < 8 && c >= 0 && c < 8) {
            auto piece = board.getPiece(r, c);
            if (!piece) {
                moves.emplace_back(r, c);
            } else {
                if (piece->isWhite() != white)
                    moves.emplace_back(r, c);
                break;
            }
            r += d[0];
            c += d[1];
        }
    }
    return moves;
}

vector<pair<int, int>> Queen::getLegalMoves(int row, int col, const Board& board) const {
    // Create explicit rook and bishop objects.
    Rook rook(white);
    Bishop bishop(white);
    vector<pair<int, int>> moves = rook.getLegalMoves(row, col, board);
    vector<pair<int, int>> bishopMoves = bishop.getLegalMoves(row, col, board);
    moves.insert(moves.end(), bishopMoves.begin(), bishopMoves.end());
    return moves;
}

vector<pair<int, int>> King::getLegalMoves(int row, int col, const Board& board) const {
    vector<pair<int, int>> moves;
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0)
                continue;
            int r = row + dr, c = col + dc;
            if (r >= 0 && r < 8 && c >= 0 && c < 8) {
                auto piece = board.getPiece(r, c);
                if (!piece || piece->isWhite() != white)
                    moves.emplace_back(r, c);
            }
        }
    }

    // Castling moves.
    if (!board.hasKingMoved(white)) {
        if (!board.getPiece(row, col + 1) &&
            !board.getPiece(row, col + 2)) {
            moves.emplace_back(row, col + 2);
        }
        if (!board.getPiece(row, col - 1) &&
            !board.getPiece(row, col - 2) &&
            !board.getPiece(row, col - 3)) {
            moves.emplace_back(row, col - 2);
        }
    }

    return moves;
}
