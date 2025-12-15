#include "Bitboard.h"
#include "Evaluation.h"

double Evaluation::evaluate(Bitboard& bb) 
{
    int game_phase = 0;
    double score = 0;

    // game phase calcs
    game_phase += __builtin_popcountll(bb.wknights);
    game_phase += __builtin_popcountll(bb.wbishops);
    game_phase += __builtin_popcountll(bb.wrooks) * 2;
    game_phase += __builtin_popcountll(bb.wqueens) * 4;

    game_phase += __builtin_popcountll(bb.bknights);
    game_phase += __builtin_popcountll(bb.bbishops);
    game_phase += __builtin_popcountll(bb.brooks) * 2;
    game_phase += __builtin_popcountll(bb.bqueens) * 4;

    // material scoring
    score += score_piece(bb.wpawns,   0,   WHITE, game_phase);
    score += score_piece(bb.wknights, 1, WHITE, game_phase);
    score += score_piece(bb.wbishops, 2, WHITE, game_phase);
    score += score_piece(bb.wrooks,   3,   WHITE, game_phase);
    score += score_piece(bb.wqueens,  4,  WHITE, game_phase);
    score += score_piece(bb.wking,    5,   WHITE, game_phase);

    score -= score_piece(bb.bpawns,   0,   BLACK, game_phase);
    score -= score_piece(bb.bknights, 1, BLACK, game_phase);
    score -= score_piece(bb.bbishops, 2, BLACK, game_phase);
    score -= score_piece(bb.brooks,   3,   BLACK, game_phase);
    score -= score_piece(bb.bqueens,  4,  BLACK, game_phase);
    score -= score_piece(bb.bking,    5,   BLACK, game_phase);

    score += connected_pawn_bonus(bb.wpawns, WHITE);
    score -= connected_pawn_bonus(bb.bpawns, BLACK);
    score -= pawn_structure_penalty(bb.wpawns, WHITE);
    score += pawn_structure_penalty(bb.bpawns, BLACK);

    return score/100;
}

double Evaluation::score_piece(U64 bb, int piece_type, int color, int game_phase) 
{
    double mg_score = 0, eg_score = 0;

    // interpolation weights
    double mg_weight = (double)game_phase / MAX_GAME_PHASE;
    if (mg_weight > 1.0) mg_weight = 1.0;
    double eg_weight = 1.0 - mg_weight;

    while (bb) {
        int sq = __builtin_ctzll(bb);
        bb &= bb - 1;

        int idx = (color == WHITE ? sq : sq ^ 56); //using sq ^ 56 to make it faster with only vertical rotation versus ur original which is 63-sq and does horizontal shift (incorrect)

        mg_score += PIECE_VALUES[0][piece_type] + midgame_PST[piece_type][idx];
        eg_score += PIECE_VALUES[1][piece_type] + endgame_PST[piece_type][idx];
    }

    return mg_score * mg_weight + eg_score * eg_weight;
}
int Evaluation::connected_pawn_bonus(U64 pawns, int color) 
{
    int bonus = 0;

    if (color == WHITE){
        U64 left_connected = pawns & ((pawns & ~FILE_A) << 7); //not File A
        U64 right_connected = pawns & ((pawns & ~FILE_H) << 9); //not File H
        bonus += __builtin_popcount(left_connected) * CONNECTED_PAWN_SCORE;
        bonus += __builtin_popcount(right_connected) * CONNECTED_PAWN_SCORE; 
    }
    else{
        U64 left_connected = pawns & ((pawns & ~FILE_A) >> 9); //not File A
        U64 right_connected = pawns & ((pawns & ~FILE_H) >> 7); //not File H
        bonus += __builtin_popcount(left_connected) * CONNECTED_PAWN_SCORE;
        bonus += __builtin_popcount(right_connected) * CONNECTED_PAWN_SCORE; 
    }
    return bonus;
}
int Evaluation::pawn_structure_penalty(U64 pawns, int color) 
{
    int penalty = 0;

    //double pawns penalty
    U64 doubled = (color==0) ? (pawns & (pawns << 8)) : (pawns & (pawns >> 8));
    U64 file_mask = ((pawns & FILE_A) != 0 ? 1ULL : 0ULL) |
    ((pawns & FILE_B) != 0 ? 2ULL : 0ULL) |
    ((pawns & FILE_C) != 0 ? 4ULL : 0ULL) |
    ((pawns & FILE_D) != 0 ? 8ULL : 0ULL) |
    ((pawns & FILE_E) != 0 ? 16ULL : 0ULL) |
    ((pawns & FILE_F) != 0 ? 32ULL : 0ULL) |
    ((pawns & FILE_G) != 0 ? 64ULL : 0ULL) |
    ((pawns & FILE_H) != 0 ? 128ULL : 0ULL);

    long isolated_files = file_mask & ~(file_mask << 1) & ~(file_mask >> 1);

    for (int f = 0; f < 8; f++){
        if ((isolated_files & (1ULL << f)) != 0){
            penalty += __builtin_popcount(pawns & FILES[f]) * ISOLATED_PAWN_SCORE;
        }
    }

    penalty += __builtin_popcount(doubled) * DOUBLE_PAWN_SCORE;
    return penalty;
}
