#include "Bitboard.h"
#include "Evaluation.h"
double evaluate(Bitboard& bb, bool isWhiteToMove) {
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


    return score;
}

double scorePiece(U64 bb, int pieceType, int color, int gamePhase) {
    double mgScore = 0, egScore = 0;

    // interpolation weights
    double mgWeight = (double)gamePhase / MAX_GAME_PHASE;
    if (mgWeight > 1.0) mgWeight = 1.0;
    double egWeight = 1.0 - mgWeight;

    while (bb) {
        int sq = __builtin_ctzll(bb);
        bb &= bb - 1;

        int idx = (color == WHITE ? sq : 63 - sq);

        mgScore += PIECE_VALUES[0][pieceType] + midgamePST[pieceType][idx];
        egScore += PIECE_VALUES[1][pieceType] + endgamePST[pieceType][idx];
    }

    return mgScore * mgWeight + egScore * egWeight;
}
int connectedPawnBonus(U64 pawns, int color) {
    int bonus = 0;

    while (pawns) {
        int sq = __builtin_ctzll(pawns);
        pawns &= pawns - 1;

        int file = sq % 8;

        bool leftFile = (file > 0);
        bool rightFile = (file < 7);

        // masks for adjacent files
        U64 leftMask  = leftFile  ? (0x0101010101010101ULL << (file - 1)) : 0;
        U64 rightMask = rightFile ? (0x0101010101010101ULL << (file + 1)) : 0;

        if (((pawns & leftMask) != 0) || ((pawns & rightMask) != 0)) {
            bonus += CONNECTED_PAWN_BONUS;
        }
    }

    return bonus;
}
int pawnStructurePenalty(U64 pawns, int color) {
    int penalty = 0;

    U64 original = pawns;

    // count file occupancy
    int fileCount[8] = {0};

    U64 tmp = pawns;
    while (tmp) {
        int sq = __builtin_ctzll(tmp);
        tmp &= tmp - 1;

        int file = sq % 8;
        fileCount[file]++;
    }

    // doubled pawn penalty
    for (int f = 0; f < 8; f++) {
        if (fileCount[f] > 1) {
            penalty += DOUBLE_PAWN_PENALTY * (fileCount[f] - 1);
        }
    }

    // isolated pawn penalty
    while (pawns) {
        int sq = __builtin_ctzll(pawns);
        pawns &= pawns - 1;

        int file = sq % 8;

        bool hasLeft  = (file > 0 && fileCount[file - 1] > 0);
        bool hasRight = (file < 7 && fileCount[file + 1] > 0);

        if (!hasLeft && !hasRight) {
            penalty += ISOLATED_PAWN_PENALTY;
        }
    }

    return penalty;
}
