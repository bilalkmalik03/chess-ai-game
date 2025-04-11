#pragma once
#include "Board.hpp"
#include <tuple>
#include <vector>
using namespace std;

class Bot {
public:
    // Makes the best move on the board using a minimax search with alpha-beta pruning.
    // 'depth' controls the search depth and should increase with difficulty.
    // 'isWhiteBot' indicates whether the bot is playing as white.
    static void makeMove(Board& board, int depth, bool isWhiteBot);

private:
    // Evaluates the board: returns a score in centipawns, where positive favors White and negative favors Black.
    static int evaluate(Board& board);

    // Returns all legal moves for the given side (true for white, false for black).
    // Each move is represented as a tuple: (fromRow, fromCol, toRow, toCol).
    static vector<tuple<int, int, int, int>> getAllLegalMoves(Board& board, bool white);

    // A recursive minimax search using alpha-beta pruning.
    // 'depth' is the remaining search depth.
    // 'alpha' and 'beta' are the bounds for pruning.
    // 'maximizingPlayer' indicates if the current node is maximizing.
    // 'isWhiteBot' indicates the bot's color.
    static int alphabeta(Board& board, int depth, int alpha, int beta, bool maximizingPlayer, bool isWhiteBot);
};
