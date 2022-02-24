#include "piece_creator.h"
#include "chess_piece_creator.h"
#include "piece.h"
#include "chess_piece.h"
#include "chess_instruction.h"

ChessPiece* ChessPieceCreator::generate(const ChessISA& instruct) {
    switch (instruct.pieceType) {
    case PieceType::Rook:
        return new Rook{instruct.group, instruct.pieceType};

    case PieceType::Bishop:
        return new Bishop{instruct.group, instruct.pieceType};

    case PieceType::Queen:
        return new Queen{instruct.group, instruct.pieceType};

    case PieceType::King:
        return new King{instruct.group, instruct.pieceType};

    case PieceType::Knight:
        return new Knight{instruct.group, instruct.pieceType};

    case PieceType::Pawn:
        return new Pawn{instruct.group, instruct.pieceType};

    default:
        return nullptr;
    }

}

