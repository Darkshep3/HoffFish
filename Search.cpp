#include "Search.h"
using namespace std;
#include <algorithm> 
#include <iostream>

Search::Search() : time_limit_ms(10000), time_up(false), search_depth(5){}

Search::Search (int time_limit) : time_limit_ms(time_limit), time_up(false), search_depth(5){}

void Search::set_search_depth(int depth)
{
    search_depth = depth;
}
void Search::set_time_limit_ms(int ms) 
{
    time_limit_ms = ms;
}
double Search::alpha_beta(GameState& game, int depth, double alpha, double beta, int max_depth) 
{

    if (time_exceeded())
    {
        return Evaluation::evaluate(game.bb);
    }
    
    if (depth == 0) 
    {
        return Evaluation::evaluate(game.bb);
    }
    auto moves = MoveGenerator::generate_legal_moves(game);

    if (moves.empty())
    {
        if (MoveGenerator::is_in_check(game.white_to_move, game.bb))
        {
            return game.white_to_move? -CHECKMATE_SCORE + (max_depth-depth) : CHECKMATE_SCORE - (max_depth - depth);
        }
        else 
            return DRAW_SCORE;
    }
    order_moves(moves);

    bool maximizing = game.white_to_move;
    if (maximizing) 
    {
        double value = -1e9;
        for (Move& move : moves) 
        {
            //Also modified your previous approach as creating a new gamestate instead of delta is alot more inefficient
            Delta d = game.delta_move(move);
            double score = alpha_beta(game, depth - 1, alpha, beta, max_depth);
            game.unmake_move(d);
            if (score > value) 
            {
                value = score;
            }
            // value = max(value, alphabeta(new_game, depth - 1, alpha, beta, false));
            if (value >= beta) 
            {
                break;
            }
            alpha = max(alpha, value);
        }
        return value;
    } else 
    {
        double value = 1e9;
        Move best_move = Move(-1,-1);
        for (Move& move : moves) 
        {
            Delta d = game.delta_move(move);
            double score = alpha_beta(game, depth - 1, alpha, beta, max_depth);
            game.unmake_move(d);
            if (score < value) 
            {
                value = score;
            }
            // value = min(value, alphabeta(new_game, depth - 1, alpha, beta, true));
            if (value <= alpha) 
            {
                break;
            }
            beta = min(beta, value);
        }
        return value;
    }
}   

int Search::get_move_priority(Move m)
{
    MoveType move_type = m.get_move_type();
    if (m.is_promotion()) return 3;
    else if (move_type == MoveType::CAPTURES) return 2;
    else if (move_type == MoveType::EN_PASSANT) return 1;
    return 0;
}

void Search::order_moves (vector<Move> &moves)
{
    std::sort(moves.begin(), moves.end(), [] (const Move& m1, const Move& m2)
    {
        return get_move_priority(m1) > get_move_priority(m2);
    });
}

Move Search::alpha_beta_root(GameState& game, int depth)
{
    vector <Move> legalMoves = MoveGenerator::generate_legal_moves(game);
    order_moves(legalMoves);

    Move best_move;
    bool max_player = game.white_to_move;
    double best_score = max_player ? -1e9 : 1e9;
    double alpha = -1e9;
    double beta = 1e9;

    for (Move& move: legalMoves)
    {
        if (time_exceeded()){
            break;
        }
        Delta delta = game.delta_move(move);
        double score = alpha_beta(game, depth-1, alpha, beta, depth);
        game.unmake_move(delta);
        if (max_player){
            if (score > best_score)
            {
                best_score = score;
                best_move = move;
            }
            alpha = max(alpha, score);
        }
        else{
            if (score < best_score) 
            {
                best_score = score;
                best_move = move;
            }
            beta = min(beta, score);
        }

    }
    //cout << "Eval: " << game.white_to_move ? best_score: -best_score;

    return best_move; 
}

Move Search::find_best_move_iterative(GameState& game, int max_depth)
{
    start_time = timer::now();
    time_up = false;
    Move best_move = Move();
    for (int depth = 1; depth <= max_depth; depth++)
    {
        Move currentBestMove = alpha_beta_root(game, depth);
        if (!time_up)
            best_move = currentBestMove;
        else 
            break;
    }

    return best_move;
}

Move Search::find_best_move (GameState& game, int max_depth)
{
    return find_best_move_iterative(game, max_depth);
}

bool Search::time_exceeded()
{
    if (time_up) return true;
    auto now = timer::now();
    auto ms = chrono::duration_cast<chrono::milliseconds>(now-start_time).count();
    if (ms > time_limit_ms)
    {
        time_up = true;
        return true;
    }
    return false;
}