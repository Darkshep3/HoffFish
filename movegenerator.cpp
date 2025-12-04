#include "movegenerator.h"
bool MoveGenerator::isAttackedSquare(Bitboard& bb, int square, bool isWhite){
    //in progress
    return true;
}

vector<Move> MoveGenerator::generatePseudoMoves(const GameState& state)
{
    const Bitboard& board = state.bb;
    //list of moves we return
    vector<Move> moves;
    //find active color
    if(state.white_to_move)
    {
        //enemy bb
        U64 enemy = board.bpawns | board.bknights | board.bbishops | board.brooks | board.bqueens | board.bking;
        //ally bb
        U64 ally = board.wpawns | board.wknights | board.wbishops | board.wrooks | board.wqueens | board.wking;
        //empty bb
        U64 empty = ~(ally | enemy);  
        //loop through all that side’s pieces

        //pawns
        generatePawnMoves(board, enemy, empty, state.en_passant, true, moves);
        //true bc white is already checked to be active here

        //knights
        generateKnightMoves(board, ally, true, moves);

        //bishops
        generateBishopMoves(board, enemy, empty, true, moves);

        //rooks
        //to be coded
        generateRookMoves(board, enemy, empty, true, moves);

        //queens
        //to be coded
        generateQueenMoves(board, enemy, empty, true, moves);

        //king
        generateKingMoves(board, ally, empty, true, state.castleWK, state.castleWQ, moves);
        
        
    }else
    {
        //ally bb
        U64 ally = board.bpawns | board.bknights | board.bbishops | board.brooks | board.bqueens | board.bking;
        //enemy bb
        U64 enemy = board.wpawns | board.wknights | board.wbishops | board.wrooks | board.wqueens | board.wking;
        //empty bb
        U64 empty = ~(ally | enemy);  
        //loop through all that side’s pieces

        //pawns
        generatePawnMoves(board, enemy, empty, state.en_passant, false, moves);
        //true bc white is already checked to be active here

        //knights
        generateKnightMoves(board, ally, false, moves);

        //bishops
        generateBishopMoves(board, enemy, empty, false, moves);

        //rooks
        //to be coded
        generateRookMoves(board, enemy, empty, false, moves);

        //queens
        //to be coded
        generateQueenMoves(board, enemy, empty, false, moves);

        //king
        generateKingMoves(board, ally, empty, false, state.castleBK, state.castleBQ, moves);
    }
    
    //return vector
    return moves;
}

bool MoveGenerator::is_in_check(bool check_for_white, Bitboard bb, GameState& state)
{
    //determine which side we are checking for (black / white)
    //our side's king's square
    int kings_sq;
    if(check_for_white)
    {
        kings_sq = get_LSB(bb.wking);
    }else
    {
        kings_sq = get_LSB(bb.bking);
    }

    //get a list of the enemy's pseudomoves
    vector<Move> pseudo = generatePseudoMoves(state);

    //if any of their "to" squares is where our king is at,
    //is_in_check = true
    //else
    //is_in_check = false
    for(Move m: pseudo)
    {
        if(m.to == kings_sq)
        {
            return true;
        }
    }

    return false;
}

vector<Move> MoveGenerator::generateLegalMoves(GameState& state)
{
    // Bitboard& board = state.bb;
    //obtain list of pseudo moves
    vector<Move> pseudo_moves = generatePseudoMoves(state);
    //empty list of legal moves
    vector<Move> legal_moves;
    for(const Move m: pseudo_moves)
    {
        //make move and check legality
        Delta delta = state.deltaMove(m);
    
        //if legal, store to legal_moves
        if(!is_in_check)
        {
            legal_moves.push_back(m);
        }
        //unmake move
        state.unmakeMove(delta);
    }
    
    return legal_moves;

}


void MoveGenerator::generateKingMoves(const Bitboard& bb, U64 allies, U64 empty, bool isWhiteToMove, bool kCastle, bool qCastle, vector<Move>& moves){
    U64 king = isWhiteToMove ? bb.wking : bb.bking;
    int fromSq = get_LSB(king);
    U64 temp = getKingAttacks(fromSq, allies);
    while (temp != 0){
        moves.push_back(Move(fromSq, get_LSB(temp)));
    }
    if (kCastle) {
        int sq = isWhiteToMove? 5: 61;
        int sq2 = isWhiteToMove? 6: 62;
        bool f = (((1ULL << (sq)) & empty) != 0) && !(isAttackedSquare(bb, sq, !isWhiteToMove));
        bool g = ((1ULL << (sq2)) & empty) != 0 && !(isAttackedSquare(bb, sq2, !isWhiteToMove));
        if (f && g) {
            int toSq = isWhiteToMove? 6: 62;
            moves.push_back(Move(fromSq, toSq, MoveType::CASTLING));
        }
    }
    if (qCastle) {
        int sq = isWhiteToMove? 1: 57;
        int sq2 = isWhiteToMove? 2: 58;
        int sq3 = isWhiteToMove? 3: 59;
        bool b = ((1ULL << (sq)) & empty) != 0;
        bool c = ((1ULL << sq2) & empty) != 0 && !(isAttackedSquare(bb, sq2, !isWhiteToMove));
        bool d = ((1ULL << sq3) & empty) != 0 && !(isAttackedSquare(bb, sq3, !isWhiteToMove));

        if (b && c && d){
            int toSq = isWhiteToMove ? 2 : 58;
            moves.push_back(Move(fromSq, toSq, MoveType::CASTLING));
        }
    }
}

//Diana 
void MoveGenerator::generateKnightMoves(const Bitboard& bb, U64 allies, bool isWhiteToMove, vector<Move>& moves)
{
   U64 knights = isWhiteToMove ? bb.wknights : bb.bknights;

   //loop thru each knight
   while(knights != 0)
   {
       int from_sq = get_LSB(knights);
       clear_LSB(knights);
       //remove current knight from bitboard


       U64 temp = getKnightMoves(from_sq, allies);


       while(temp != 0)
       {
            int to_sq = get_LSB(temp);
            clear_LSB(temp);
        

            moves.push_back(Move(from_sq,to_sq));
       }
   }
}

//Ethan  

void MoveGenerator::generatePawnMoves(const Bitboard& bb, U64 enemy, U64 empty, int enPassant, bool isWhiteToMove, vector<Move>& moves)
{
    U64 pawns = isWhiteToMove ? bb.wpawns : bb.bpawns;
    int direction = isWhiteToMove ? 8 : -8;
    int startRank = isWhiteToMove ? 1 : 6;
    int promoRank = isWhiteToMove ? 6 : 1;

    char promoQ = isWhiteToMove ? 'Q' : 'q';
    char promoR = isWhiteToMove ? 'R' : 'r';
    char promoB = isWhiteToMove ? 'B' : 'b';
    char promoN = isWhiteToMove ? 'N' : 'n';

    while (pawns != 0)
    {
        int fromSq = get_LSB(pawns);
        clear_LSB(pawns);

        int rank = fromSq / 8;
        int file = fromSq % 8;

        int oneStep = fromSq + direction;
        int twoStep = fromSq + 2*direction;
        // Pushes
        if (oneStep >= 0 && oneStep < 64 && ((empty >> oneStep) & 1ULL))
        {
            // Promotion
            if (rank == promoRank)
            {
                moves.push_back(Move(fromSq, oneStep, MoveType::PROMOTION_QUEEN, promoQ));
                moves.push_back(Move(fromSq, oneStep, MoveType::PROMOTION_ROOK,  promoR));
                moves.push_back(Move(fromSq, oneStep, MoveType::PROMOTION_BISHOP,promoB));
                moves.push_back(Move(fromSq, oneStep, MoveType::PROMOTION_KNIGHT,promoN));
            }
            else
            {
                moves.push_back(Move(fromSq, oneStep));
            }

            // Double push
            if (rank == startRank && ((empty >> twoStep) & 1ULL))
            {
                moves.push_back(Move(fromSq, twoStep));
            }
        }
        // Captures
        int capLeft = fromSq + direction - 1;
        int capRight = fromSq + direction + 1;
        // Left capture
        if (file != 0 && capLeft >= 0 && capLeft < 64 && ((enemy >> capLeft) & 1ULL))
        {
            if (rank == promoRank)
            {
                moves.push_back(Move(fromSq, capLeft, MoveType::PROMOTION_QUEEN, promoQ));
                moves.push_back(Move(fromSq, capLeft, MoveType::PROMOTION_ROOK,  promoR));
                moves.push_back(Move(fromSq, capLeft, MoveType::PROMOTION_BISHOP,promoB));
                moves.push_back(Move(fromSq, capLeft, MoveType::PROMOTION_KNIGHT,promoN));
            }
            else
            {
                moves.push_back(Move(fromSq, capLeft, MoveType::CAPTURES));
            }
        }
        // Right capture
        if (file != 7 && capRight >= 0 && capRight < 64 && ((enemy >> capRight) & 1ULL))
        {
            if (rank == promoRank)
            {
                moves.push_back(Move(fromSq, capRight, MoveType::PROMOTION_QUEEN, promoQ));
                moves.push_back(Move(fromSq, capRight, MoveType::PROMOTION_ROOK,  promoR));
                moves.push_back(Move(fromSq, capRight, MoveType::PROMOTION_BISHOP,promoB));
                moves.push_back(Move(fromSq, capRight, MoveType::PROMOTION_KNIGHT,promoN));
            }
            else
            {
                moves.push_back(Move(fromSq, capRight, MoveType::CAPTURES));
            }
        }
        // En Passant
        if (enPassant != -1)
        {
            if (capLeft == enPassant || capRight == enPassant)
            {
                moves.push_back(Move(fromSq, enPassant, MoveType::EN_PASSANT));
            }
        }
    }
}


//Arush 
//Bishop Move Generatoin 
// Rooks + queens if you have time  
void MoveGenerator::generateBishopMoves(const Bitboard& bb, U64 enemy, U64 empty, bool isWhiteToMove, vector<Move>& moves){
    U64 bishops = isWhiteToMove ? bb.wbishops : bb.bbishops;
    vector<int> fromSqs;
    while (bishops != 0) {
        fromSqs.push_back(get_LSB(bishops));
        clear_bit(bishops, get_LSB(bishops));
    }

    for (int i=0; i<fromSqs.size(); i++) {
        int fromSq = fromSqs[i];
        int rank = fromSq / 8;
        int file = fromSq % 8;
        for (int j=-1; j<=1; j+=2) {
            for (int k=-1; k<=1; k+=2) {
                int mult = 1;
                int toRank = rank + j * mult;
                int toFile = file + k * mult; 
                while (toRank >= 0 && toRank <= 7 && toFile >= 0 && toFile <= 7) {
                    if (((1ULL << (toRank * 8 + toFile)) & enemy) != 0) {
                        moves.push_back(Move(fromSq, toRank * 8 + toFile, MoveType::CAPTURES));
                        toRank = 9;
                    } else if (((1ULL << (toRank * 8 + toFile)) & empty) != 0){
                        moves.push_back(Move(fromSq, toRank * 8 + toFile, MoveType::NORMAL));
                        mult++;
                        toRank = rank + j * mult;
                        toFile = file + k * mult;                         
                    } else {
                        toRank = 9;
                    }
                }
            }
        }
    }
    //Arush 
    //generate rook moves + queens
    //queen = rook + bishops 
    /*MoveGenerator::generateQueensMoves(...){
        movegenerator:generatebishopMoves()
        movegenerator:generaterookMoves() 
    }
    */ 
}

