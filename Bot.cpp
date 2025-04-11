#include "Bot.hpp"
#include "Board.hpp"
#include "Piece.hpp"
#include <limits>
#include <tuple>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

//---------------------------------------------------------------------
// Evaluation Function
//---------------------------------------------------------------------
// A simple material-based evaluation in centipawns. Positive values favor White; negative values favor Black.
int Bot::evaluate(Board& board) {
    int score = 0;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Piece* piece = board.getPiece(row, col);
            if (!piece)
                continue;
            int pieceValue = 0;
            switch (toupper(piece->getSymbol())) {
                case 'P': pieceValue = 100; break;
                case 'N': 
                case 'B': pieceValue = 320; break;
                case 'R': pieceValue = 500; break;
                case 'Q': pieceValue = 900; break;
                case 'K': pieceValue = 20000; break;
                default: break;
            }
            score += piece->isWhite() ? pieceValue : -pieceValue;
        }
    }
    return score;
}

//---------------------------------------------------------------------
// Generate All Legal Moves
//---------------------------------------------------------------------
// Returns all legal moves for the given side (true for white, false for black).
// Each move is represented as a tuple (fromRow, fromCol, toRow, toCol).
vector<tuple<int, int, int, int>> Bot::getAllLegalMoves(Board& board, bool white) {
    vector<tuple<int, int, int, int>> moves;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = board.getPiece(r, c);
            if (p && p->isWhite() == white) {
                auto candidateMoves = p->getLegalMoves(r, c, board);
                for (auto& m : candidateMoves) {
                    Board temp = board;
                    temp.movePiece(r, c, m.first, m.second);
                    if (!temp.isInCheck(white))
                        moves.emplace_back(r, c, m.first, m.second);
                }
            }
        }
    }
    return moves;
}

//---------------------------------------------------------------------
// Alpha-Beta Pruning with Minimax
//---------------------------------------------------------------------
// Recursive minimax search enhanced with alpha-beta pruning.
// 'depth' is the remaining depth to search.
// 'alpha' and 'beta' are the current bounds for pruning.
// 'maximizingPlayer' indicates whether this node is maximizing or minimizing.
// 'isWhiteBot' indicates the bot's side.
int Bot::alphabeta(Board& board, int depth, int alpha, int beta, bool maximizingPlayer, bool isWhiteBot) {
    if (depth == 0 || board.isCheckmate(true) || board.isCheckmate(false) ||
        board.isStalemate(true) || board.isStalemate(false))
    {
        return evaluate(board);
    }
    
    bool currentColor = maximizingPlayer ? isWhiteBot : !isWhiteBot;
    auto moves = getAllLegalMoves(board, currentColor);

    // If no legal moves exist, return evaluation.
    if (moves.empty())
        return evaluate(board);

    if (maximizingPlayer) {
        int value = numeric_limits<int>::min();
        for (auto& move : moves) {
            Board temp = board;
            temp.movePiece(get<0>(move), get<1>(move), get<2>(move), get<3>(move));
            int score = alphabeta(temp, depth - 1, alpha, beta, false, isWhiteBot);
            value = max(value, score);
            alpha = max(alpha, value);
            if (alpha >= beta)
                break; // Beta cutoff.
        }
        return value;
    } else {
        int value = numeric_limits<int>::max();
        for (auto& move : moves) {
            Board temp = board;
            temp.movePiece(get<0>(move), get<1>(move), get<2>(move), get<3>(move));
            int score = alphabeta(temp, depth - 1, alpha, beta, true, isWhiteBot);
            value = min(value, score);
            beta = min(beta, value);
            if (beta <= alpha)
                break; // Alpha cutoff.
        }
        return value;
    }
}

//---------------------------------------------------------------------
// Make Move Using Minimax with Alpha-Beta Pruning
//---------------------------------------------------------------------
// Iterates through all legal moves and uses the alphabeta function to determine
// the best move. The depth parameter is set from the main program and increases
// with difficulty.
void Bot::makeMove(Board& board, int depth, bool isWhiteBot) {
    auto moves = getAllLegalMoves(board, isWhiteBot);
    if (moves.empty())
        return;

    // Optional: Shuffle moves to add variety when moves evaluate equally.
    random_device rd;
    mt19937 g(rd());
    shuffle(moves.begin(), moves.end(), g);

    int bestScore = numeric_limits<int>::min();
    tuple<int, int, int, int> bestMove = moves[0];

    for (auto& move : moves) {
        Board temp = board;
        temp.movePiece(get<0>(move), get<1>(move), get<2>(move), get<3>(move));
        int score = alphabeta(temp, depth - 1, numeric_limits<int>::min(), numeric_limits<int>::max(), false, isWhiteBot);
        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }
    board.movePiece(get<0>(bestMove), get<1>(bestMove), get<2>(bestMove), get<3>(bestMove));
}

