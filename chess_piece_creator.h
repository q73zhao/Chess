#ifndef _CHESS_PIECE_CREATOR_H_
#define _CHESS_PIECE_CREATOR_H_
#include <iostream>
#include <vector>
#include "piece_creator.h"
#include "chess_piece.h"
class ChessISA;
class ChessPieceCreator : public PieceCreator<ChessISA> {
    ChessPiece* generate(const ChessISA&) override;
};

#endif

