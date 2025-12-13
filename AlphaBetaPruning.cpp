#include "AlphaBetaPruning.h"
using namespace std;

// function alphabeta(node, depth, α, β, maximizingPlayer) is
//     if depth == 0 or node is terminal then
//         return the heuristic value of node
//     if maximizingPlayer then
//         value := −∞
//         for each child of node do
//             value := max(value, alphabeta(child, depth − 1, α, β, FALSE))
//             if value ≥ β then
//                 break (* β cutoff *)
//             α := max(α, value)
//         return value
//     else
//         value := +∞
//         for each child of node do
//             value := min(value, alphabeta(child, depth − 1, α, β, TRUE))
//             if value ≤ α then
//                 break (* α cutoff *)
//             β := min(β, value)
//         return value
// (* Initial call *)
// alphabeta(origin, depth, −∞, +∞, TRUE)

SearchResult alphabeta(GameState game, int depth, double alpha, double beta, bool maximizing_player) {
    if (depth == 0 || MoveGenerator::generateLegalMoves(game).size() == 0) {
        return {Evaluation::evaluate(game.bb), Move(-1,-1)};
    }
    if (maximizing_player) {
        double value = INT_MIN;
        Move best_move = Move(-1,-1);
        for (Move move : MoveGenerator::generateLegalMoves(game)) {
            GameState new_game = game;
            new_game.makeMove(move);
            auto [new_value, new_move] = alphabeta(new_game, depth - 1, alpha, beta, false);
            if (new_value > value) {
                value = new_value;
                best_move = move;
            }
            // value = max(value, alphabeta(new_game, depth - 1, alpha, beta, false));
            if (value >= beta) {
                break;
            }
            alpha = max(alpha, value);
        }
        return {value, best_move};
    } else {
        double value = INT_MAX;
        Move best_move = Move(-1,-1);
        for (Move move : MoveGenerator::generateLegalMoves(game)) {
            GameState new_game = game;
            new_game.makeMove(move);
            auto [new_value, new_move] = alphabeta(new_game, depth - 1, alpha, beta, true);
            if (new_value < value) {
                value = new_value;
                best_move = move;
            }
            // value = min(value, alphabeta(new_game, depth - 1, alpha, beta, true));
            if (value <= alpha) {
                break;
            }
            beta = min(beta, value);
        }
        return {value, best_move};
    }
}   
