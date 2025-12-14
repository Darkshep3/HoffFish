#include "Bitboard.h"
#include "Evaluation.h"

double Evaluation::evaluate(Bitboard& bb) {
    int gamePhase = 0;
    double score = 0;

    // game phase calcs
    gamePhase += __builtin_popcountll(bb.wknights);
    gamePhase += __builtin_popcountll(bb.wbishops);
    gamePhase += __builtin_popcountll(bb.wrooks) * 2;
    gamePhase += __builtin_popcountll(bb.wqueens) * 4;

    gamePhase += __builtin_popcountll(bb.bknights);
    gamePhase += __builtin_popcountll(bb.bbishops);
    gamePhase += __builtin_popcountll(bb.brooks) * 2;
    gamePhase += __builtin_popcountll(bb.bqueens) * 4;

    // material scoring
    score += scorePiece(bb.wpawns,   0,   WHITE, gamePhase);
    score += scorePiece(bb.wknights, 1, WHITE, gamePhase);
    score += scorePiece(bb.wbishops, 2, WHITE, gamePhase);
    score += scorePiece(bb.wrooks,   3,   WHITE, gamePhase);
    score += scorePiece(bb.wqueens,  4,  WHITE, gamePhase);
    score += scorePiece(bb.wking,    5,   WHITE, gamePhase);

    score -= scorePiece(bb.bpawns,   0,   BLACK, gamePhase);
    score -= scorePiece(bb.bknights, 1, BLACK, gamePhase);
    score -= scorePiece(bb.bbishops, 2, BLACK, gamePhase);
    score -= scorePiece(bb.brooks,   3,   BLACK, gamePhase);
    score -= scorePiece(bb.bqueens,  4,  BLACK, gamePhase);
    score -= scorePiece(bb.bking,    5,   BLACK, gamePhase);

    score += connectedPawnBonus(bb.wpawns, WHITE);
    score -= connectedPawnBonus(bb.bpawns, BLACK);
    score -= pawnStructurePenalty(bb.wpawns, WHITE);
    score += pawnStructurePenalty(bb.bpawns, BLACK);

    return score/100;
}

double Evaluation::scorePiece(U64 bb, int pieceType, int color, int gamePhase) {
    double mgScore = 0, egScore = 0;

    // interpolation weights
    double mgWeight = (double)gamePhase / MAX_GAME_PHASE;
    if (mgWeight > 1.0) mgWeight = 1.0;
    double egWeight = 1.0 - mgWeight;

    while (bb) {
        int sq = __builtin_ctzll(bb);
        bb &= bb - 1;

        int idx = (color == WHITE ? sq : sq ^ 56); //using sq ^ 56 to make it faster with only vertical rotation versus ur original which is 63-sq and does horizontal shift (incorrect)

        mgScore += PIECE_VALUES[0][pieceType] + midgamePST[pieceType][idx];
        egScore += PIECE_VALUES[1][pieceType] + endgamePST[pieceType][idx];
    }

    return mgScore * mgWeight + egScore * egWeight;
}
int Evaluation::connectedPawnBonus(U64 pawns, int color) {
    int bonus = 0;

    if (color == WHITE){
        U64 leftConnected = pawns & ((pawns & ~FILE_A) << 7); //not File A
        U64 rightConnected = pawns & ((pawns & ~FILE_H) << 9); //not File H
        bonus += __builtin_popcount(leftConnected) * CONNECTED_PAWN_SCORE;
        bonus += __builtin_popcount(rightConnected) * CONNECTED_PAWN_SCORE; 
    }
    else{
        U64 leftConnected = pawns & ((pawns & ~FILE_A) >> 9); //not File A
        U64 rightConnected = pawns & ((pawns & ~FILE_H) >> 7); //not File H
        bonus += __builtin_popcount(leftConnected) * CONNECTED_PAWN_SCORE;
        bonus += __builtin_popcount(rightConnected) * CONNECTED_PAWN_SCORE; 
    }
    return bonus;
}
int Evaluation::pawnStructurePenalty(U64 pawns, int color) {
    int penalty = 0;

    //double pawns penalty
    U64 doubled = (color==0) ? (pawns & (pawns << 8)) : (pawns & (pawns >> 8));
    U64 fileMask = ((pawns & FILE_A) != 0 ? 1ULL : 0ULL) |
    ((pawns & FILE_B) != 0 ? 2ULL : 0ULL) |
    ((pawns & FILE_C) != 0 ? 4ULL : 0ULL) |
    ((pawns & FILE_D) != 0 ? 8ULL : 0ULL) |
    ((pawns & FILE_E) != 0 ? 16ULL : 0ULL) |
    ((pawns & FILE_F) != 0 ? 32ULL : 0ULL) |
    ((pawns & FILE_G) != 0 ? 64ULL : 0ULL) |
    ((pawns & FILE_H) != 0 ? 128ULL : 0ULL);

    long isolatedFiles = fileMask & ~(fileMask << 1) & ~(fileMask >> 1);

    for (int f = 0; f < 8; f++){
        if ((isolatedFiles & (1ULL << f)) != 0){
            penalty += __builtin_popcount(pawns & FILES[f]) * ISOLATED_PAWN_SCORE;
        }
    }

    penalty += __builtin_popcount(doubled) * DOUBLE_PAWN_SCORE;
    return penalty;
}
