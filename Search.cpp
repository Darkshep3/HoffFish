#include "Search.h"
using namespace std;
#include <algorithm> 
#include <iostream>

Search::Search() : timeLimitMs(10000), timeUp(false), searchDepth(5){}

Search::Search (int time_limit) : timeLimitMs(time_limit), timeUp(false), searchDepth(5){}

void Search::setSearchDepth(int depth){
    searchDepth = depth;
}

double Search::alphabeta(GameState& game, int depth, double alpha, double beta, int maxDepth) {

    if (timeExceeded()){
        return Evaluation::evaluate(game.bb);
    }
    
    if (depth == 0) {
        return Evaluation::evaluate(game.bb);
    }
    auto moves = MoveGenerator::generateLegalMoves(game);

    if (moves.empty()){
        if (MoveGenerator::is_in_check(game.white_to_move, game.bb)){
            return game.white_to_move? -CHECKMATE_SCORE + (maxDepth-depth) : CHECKMATE_SCORE - (maxDepth - depth);
        }
        else 
            return DRAW_SCORE;
    }

    bool maximizing = game.white_to_move;
    if (maximizing) {
        double value = -1e9;
        for (Move move : moves) {
            //Also modified your previous approach as creating a new gamestate instead of delta is alot more inefficient
            Delta d = game.deltaMove(move);
            double score = alphabeta(game, depth - 1, alpha, beta, maxDepth);
            game.unmakeMove(d);
            if (score > value) {
                value = score;
            }
            // value = max(value, alphabeta(new_game, depth - 1, alpha, beta, false));
            if (value >= beta) {
                break;
            }
            alpha = max(alpha, value);
        }
        return value;
    } else {
        double value = 1e9;
        Move best_move = Move(-1,-1);
        for (Move move : moves) {
            Delta d = game.deltaMove(move);
            double score = alphabeta(game, depth - 1, alpha, beta, maxDepth);
            game.unmakeMove(d);
            if (score < value) {
                value = score;
            }
            // value = min(value, alphabeta(new_game, depth - 1, alpha, beta, true));
            if (value <= alpha) {
                break;
            }
            beta = min(beta, value);
        }
        return value;
    }
}   

int Search::getMovePriority(Move m){
    MoveType moveType = m.getMoveType();
    if (m.isPromotion()) return 3;
    else if (moveType == MoveType::CAPTURES) return 2;
    else if (moveType == MoveType::EN_PASSANT) return 1;
    return 0;
}

void Search::orderMoves (vector<Move> &moves){
    std::sort(moves.begin(), moves.end(), [] (const Move& m1, const Move& m2){
        return getMovePriority(m1) > getMovePriority(m2);
    });
}

Move Search::alphaBetaRoot(GameState& game, int depth){
    vector <Move> legalMoves = MoveGenerator::generateLegalMoves(game);
    orderMoves(legalMoves);

    Move bestMove;
    bool max_player = game.white_to_move;
    double bestScore = max_player ? -1e9 : 1e9;
    double alpha = -1e9;
    double beta = 1e9;

    for (Move move: legalMoves){
        if (timeExceeded()){
            break;
        }
        Delta delta = game.deltaMove(move);
        double score = alphabeta(game, depth-1, alpha, beta, depth);
        game.unmakeMove(delta);
        if (max_player){
            if (score > bestScore){
                bestScore = score;
                bestMove = move;
            }
            alpha = max(alpha, score);
        }
        else{
            if (score < bestScore) {
                bestScore = score;
                bestMove = move;
            }
            beta = min(beta, score);
        }

    }
    //cout << "Eval: " << game.white_to_move ? bestScore: -bestScore;

    return bestMove; 
}

Move Search::findBestMoveIterative(GameState& game, int maxDepth){
    startTime = timer::now();
    timeUp = false;
    Move bestMove = Move();
    for (int depth = 1; depth <= maxDepth; depth++){
        Move currentBestMove = alphaBetaRoot(game, depth);
        if (!timeUp)
            bestMove = currentBestMove;
        else 
            break;
    }

    return bestMove;
}

Move Search::findBestMove (GameState& game, int maxDepth){
    return findBestMoveIterative(game, maxDepth);
}

bool Search::timeExceeded(){
    if (timeUp) return true;
    auto now = timer::now();
    auto ms = chrono::duration_cast<chrono::milliseconds>(now-startTime).count();
    if (ms > timeLimitMs){
        timeUp = true;
        return true;
    }
    return false;
}