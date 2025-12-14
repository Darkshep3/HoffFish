#include "movegenerator.h"

 bool MoveGenerator::isAttackedSquare(Bitboard& bb, int square, bool check_for_white){
    //allies bit board
    U64 allies = check_for_white ? bb.getWhitePieces() : bb.getBlackPieces();
    
    //enemies bb
    U64 enemies = !check_for_white ? bb.getWhitePieces() : bb.getBlackPieces();
    

    //get a list of the enemy's attack sqaures as a bitboard
    //enemy king
    U64 king_sq = get_LSB(check_for_white ? bb.bking : bb.wking); 
    U64 king_attacks = getKingAttacks(king_sq, enemies); // enemies here = other side's king's allies
    //enemy knight
    U64 knights = check_for_white ? bb.bknights : bb.wknights;
    U64 knights_attacks = 0ULL; //all knights' attacks
    while(knights != 0)
    {
        int sq = get_LSB(knights);
        U64 current_attacks = getKnightAttacks(sq, enemies); //one knight's attacks
        knights_attacks = knights_attacks | current_attacks; //adding it to all knights' bb
        clear_LSB(knights); //moves on to the next knight
    }
    //enemy rook
    U64 rooks = check_for_white ? bb.brooks : bb.wrooks;
    U64 rooks_attacks = 0ULL;
    while(rooks != 0)
    {
        int sq = get_LSB(rooks);
        U64 current_attacks = getRookAttacks(sq, bb.getOccupied(), 0ULL);
        rooks_attacks = rooks_attacks | current_attacks;
        clear_LSB(rooks);
    }
    //enemy bishop
    U64 bishops = check_for_white ? bb.bbishops : bb.wbishops;
    U64 bishops_attacks = 0ULL;
    while(bishops != 0)
    {
        int sq = get_LSB(bishops);
        U64 current_attacks = getBishopAttacks(sq, bb.getOccupied(), 0ULL);
        bishops_attacks = bishops_attacks | current_attacks;
        clear_LSB(bishops);
    }
    //enemy queen
    U64 queens = check_for_white ? bb.bqueens : bb.wqueens;
    U64 queens_attacks = 0ULL;
    while(queens != 0)
    {
        int sq = get_LSB(queens);
        U64 current_attacks = getQueenAttacks(sq, bb.getOccupied(), 0ULL);
        queens_attacks = queens_attacks | current_attacks;
        clear_LSB(queens);
    }
    //enemy pawn
    //no loops, just shift
    U64 pawns = check_for_white ? bb.bpawns : bb.wpawns;

    const U64 notA = 0xfefefefefefefefeULL;
    const U64 notH = 0x7f7f7f7f7f7f7f7fULL;

    U64 pawns_attacks_left = check_for_white ? (pawns & notH) >> 7 : ( pawns & notA) << 7; 
    U64 pawns_attacks_right = check_for_white ? (pawns & notA) >> 9 : (pawns & notH) << 9;

    U64 pawns_attacks = pawns_attacks_left | pawns_attacks_right;

    //combine all those bitboard and make a enemy's_attack bitboard
    U64 enemy_attacks = king_attacks | knights_attacks | rooks_attacks | bishops_attacks | queens_attacks | pawns_attacks;

    //if king is on any of those squares in the bb -> true
    while(enemy_attacks != 0ULL)
    {
        if(square == get_LSB(enemy_attacks))
        {
            return true;
        }
        clear_LSB(enemy_attacks);
    }
    //if not -> false
    return false;
 }

vector<Move> MoveGenerator::generatePseudoMoves(GameState& state)
{
    Bitboard& bb = state.bb;
    //list of moves we return
    vector<Move> moves;
    //find active color
    if(state.white_to_move)
    {
        //enemy bb
        U64 enemy = bb.getBlackPieces();
        //ally bb
        U64 ally = bb.getWhitePieces();
        //empty bb
        U64 empty = ~(ally | enemy);  
        //loop through all that side’s pieces

        //pawns
        generatePawnMoves(bb, enemy, empty, state.en_passant, true, moves);
        //true bc white is already checked to be active here

        //knights
        generateKnightMoves(bb, ally, true, moves);

        //bishops
        generateBishopMoves(bb, ally, true, moves);

        //rooks
        generateRookMoves(bb, ally, true, moves);

        //queens
        //to be coded
        generateQueenMoves(bb, ally, true, moves);

        //king
        generateKingMoves(bb, ally, empty, true, state.castleWK, state.castleWQ, moves);
        
        
    }else
    {
        //ally bb
        U64 ally = bb.bpawns | bb.bknights | bb.bbishops | bb.brooks | bb.bqueens | bb.bking;
        //enemy bb
        U64 enemy = bb.wpawns | bb.wknights | bb.wbishops | bb.wrooks | bb.wqueens | bb.wking;
        //empty bb
        U64 empty = ~(ally | enemy);  
        //loop through all that side’s pieces

        //pawns
        generatePawnMoves(bb, enemy, empty, state.en_passant, false, moves);
        //true bc white is already checked to be active here

        //knights
        generateKnightMoves(bb, ally, false, moves);

        //bishops
        generateBishopMoves(bb, ally, false, moves);

        //rooks
        //to be coded
        generateRookMoves(bb, ally, false, moves);

        //queens
        //to be coded
        generateQueenMoves(bb, ally, false, moves);

        //king
        generateKingMoves(bb, ally, empty, false, state.castleBK, state.castleBQ, moves);
    }
    
    //return vector
    return moves;
}

bool MoveGenerator::is_in_check(bool check_for_white, Bitboard bb)
{
    //determine which side we are checking for (black / white)
    U64 king = check_for_white ? bb.wking : bb.bking;  
    int sq = get_LSB(king); 
    
    return isAttackedSquare(bb, sq, check_for_white);
}

vector<Move> MoveGenerator::generateLegalMoves(GameState& state)
{
    // Bitboard& board = state.bb;
    //obtain list of pseudo moves
    vector<Move> pseudo_moves = generatePseudoMoves(state);
    //empty list of legal moves
    vector<Move> legal_moves;

    bool isWhite = state.white_to_move;

    for(const Move& m: pseudo_moves)
    {
        //make move and check legality
        Delta delta = state.deltaMove(m);    
        //if legal, store to legal_moves
        if(!is_in_check(isWhite, state.bb))
        {
            // cout << state.white_to_move << endl;
            legal_moves.push_back(m);
        }
        //unmake move
        state.unmakeMove(delta);
    }
    
    return legal_moves;

}


void MoveGenerator::generateKingMoves(Bitboard& bb, U64 allies, U64 empty, bool isWhiteToMove, bool kCastle, bool qCastle, vector<Move>& moves){
    U64 king = isWhiteToMove ? bb.wking : bb.bking;
    int fromSq = get_LSB(king);
    U64 temp = getKingAttacks(fromSq, allies);
    while (temp != 0){
        moves.push_back(Move(fromSq, get_LSB(temp)));
        temp &= temp -1;
    }
    //ends prematuraly as king cannot castle in check

    if (isAttackedSquare(bb, fromSq, isWhiteToMove)) {
        return;
    }

    if (kCastle) {
        int sq = isWhiteToMove? 5: 61;
        int sq2 = isWhiteToMove? 6: 62;
        bool f = (((1ULL << (sq)) & empty) != 0) && !(isAttackedSquare(bb, sq, isWhiteToMove));
        bool g = (((1ULL << (sq2)) & empty) != 0) && !(isAttackedSquare(bb, sq2, isWhiteToMove));
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
        bool c = ((1ULL << sq2) & empty) != 0 && !(isAttackedSquare(bb, sq2, isWhiteToMove));
        bool d = ((1ULL << sq3) & empty) != 0 && !(isAttackedSquare(bb, sq3, isWhiteToMove));

        if (b && c && d){
            int toSq = isWhiteToMove ? 2 : 58;
            moves.push_back(Move(fromSq, toSq, MoveType::CASTLING));
        }
    }
}

//Diana 
void MoveGenerator::generateKnightMoves(Bitboard& bb, U64 allies, bool isWhiteToMove, vector<Move>& moves)
{
   U64 knights = isWhiteToMove ? bb.wknights : bb.bknights;

   //loop thru each knight
   while(knights != 0)
   {
       int from_sq = get_LSB(knights);
       clear_LSB(knights);
       //remove current knight from bitboard


       U64 temp = getKnightAttacks(from_sq, allies);


       while(temp != 0)
       {
            int to_sq = get_LSB(temp);
            clear_LSB(temp);
        

            moves.push_back(Move(from_sq,to_sq));
       }
   }
}

//Ethan  

void MoveGenerator::generatePawnMoves(Bitboard& bb, U64 enemy, U64 empty, int enPassant, bool isWhiteToMove, vector<Move>& moves)
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

void MoveGenerator::generateBishopMoves(Bitboard& bb, U64 allies, bool isWhiteToMove, vector<Move>& moves){
   U64 bishops = isWhiteToMove ? bb.wbishops : bb.bbishops;

   //loop thru each knight
   while(bishops != 0)
   {
       int from_sq = get_LSB(bishops);
       clear_LSB(bishops);

       U64 temp = getBishopAttacks(from_sq, bb.getOccupied(), allies);

       while(temp != 0)
       {
            int to_sq = get_LSB(temp);
            clear_LSB(temp);
            moves.push_back(Move(from_sq,to_sq));
       }
   }
}
void MoveGenerator::generateRookMoves(Bitboard& bb, U64 allies, bool isWhiteToMove, vector<Move>& moves){
   U64 rooks = isWhiteToMove ? bb.wrooks : bb.brooks;
   while(rooks != 0)
   {
       int from_sq = get_LSB(rooks);
       clear_LSB(rooks);

       U64 temp = getRookAttacks(from_sq, bb.getOccupied(), allies);

       while(temp != 0)
       {
            int to_sq = get_LSB(temp);
            clear_LSB(temp);
            moves.push_back(Move(from_sq,to_sq));
       }
   }
}
void MoveGenerator::generateQueenMoves(Bitboard& bb, U64 allies, bool isWhiteToMove, vector<Move>& moves){
   U64 queens = isWhiteToMove ? bb.wqueens : bb.bqueens;
   while(queens != 0)
   {
       int from_sq = get_LSB(queens);
       clear_LSB(queens);

       U64 temp = getQueenAttacks(from_sq, bb.getOccupied(), allies);

       while(temp != 0)
       {
            int to_sq = get_LSB(temp);
            clear_LSB(temp);
            moves.push_back(Move(from_sq,to_sq));
       }
   }
}

/*
//Arush 
//Bishop Move Generation
void MoveGenerator::generateBishopMoves(Bitboard& bb, U64 enemy, U64 empty, bool isWhiteToMove, vector<Move>& moves){
    //create vector of squares that contain the current player's bishops
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
}


void MoveGenerator::generateRookMoves(Bitboard& bb, U64 enemy, U64 empty, bool isWhiteToMove, vector<Move>& moves){
    //create vector of squares that contain the current player's rooks
    U64 rooks = isWhiteToMove ? bb.wrooks : bb.brooks;
    vector<int> fromSqs;
    while (rooks != 0) {
        fromSqs.push_back(get_LSB(rooks));
        clear_bit(rooks, get_LSB(rooks));
    }

    //iterate through each rook
    for (int i=0; i<fromSqs.size(); i++) {
        int fromSq = fromSqs[i];
        int rank = fromSq / 8;
        int file = fromSq % 8;

        //iterate through possible vertical moves: (j=-1 means downwards, j=1 means upwards)
        for (int j=-1; j<=1; j+=2) {
            int toRank = rank + j;
            while (toRank >= 0 && toRank <= 7) {
                if (((1ULL << (toRank * 8 + file)) & enemy) != 0) {
                    //if the square has an enemy, add capture move and exit loop
                    moves.push_back(Move(fromSq, toRank * 8 + file, MoveType::CAPTURES));
                    toRank = 9;
                } else if (((1ULL << (toRank * 8 + file)) & empty) != 0){
                    //if the square is empty, add normal move and continue iterating
                    moves.push_back(Move(fromSq, toRank * 8 + file, MoveType::NORMAL));
                    toRank += j;                      
                } else {
                    //if the square has an ally, exit loop
                    toRank = 9;
                }
            }
        }

        //iterate through possible horizontal moves: (j=-1 means left, j=1 means right)
        for (int j=-1; j<=1; j+=2) {
            int toFile = file + j;
            while (toFile >= 0 && toFile <= 7) {
                if (((1ULL << (rank * 8 + toFile)) & enemy) != 0) {
                    //if the square has an enemy, add capture move and exit loop
                    moves.push_back(Move(fromSq, rank * 8 + toFile, MoveType::CAPTURES));
                    toFile = 9;
                } else if (((1ULL << (rank * 8 + toFile)) & empty) != 0){
                    //if the square is empty, add normal move and continue iterating
                    moves.push_back(Move(fromSq, rank * 8 + toFile, MoveType::NORMAL));
                    toFile += j;                      
                } else {
                    //if the square has an ally, exit loop
                    toFile = 9;
                }
            }
        }
    }
}



void MoveGenerator::generateQueenMoves(Bitboard& bb, U64 enemy, U64 empty, bool isWhiteToMove, vector<Move>& moves){
    generateBishopMoves(bb, enemy, empty, isWhiteToMove, moves);
    generateRookMoves(bb, enemy, empty, isWhiteToMove, moves);
}

*/
