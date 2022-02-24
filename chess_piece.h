#ifndef _CHESS_PIECE_H_
#define _CHESS_PIECE_H_
#include "piece.h"
#include "chess_instruction.h"
#include <string>
#include <utility>
#include <vector>
class ChessPiece : public Piece<ChessISA> {
    Group group;
    PieceType pieceType;
    virtual std::vector<std::pair<int,int> > validMove (ChessISA instruct, const Grid<ChessISA>& grid) const = 0;
    bool validate(const ChessISA& instruct, int currPlayerIndex, const Grid<ChessISA>& grid) const;
    std::vector<std::pair<int, int>> advancedValidMove(ChessISA instruct, const Grid<ChessISA>& grid) const;
    std::vector<Posn> inBetweenPosns(Posn source, Posn destination) const ;
    bool noObstaclesAlongthePath(const ChessISA& instruct, const Grid<ChessISA>& grid) const;
    
    public:
    ChessPiece(Group, PieceType);
    ChessISA getState() const override;
    // This vector contains the result of all the places the piece can be 
    // (without King Check)
    // namely that it may led to your king being checked, which is invalid.
    std::vector<std::pair<int,int>> getRange(ChessISA, const Grid<ChessISA>&) const;
    
    
};

class Rook : public ChessPiece {
    //return the list of rook's possible moves given  the original position
    std::vector<std::pair<int,int> > validMove (ChessISA instruct,const Grid<ChessISA>& grid) const override;
    public:
    Rook(Group, PieceType);
   // std::vector<std::pair<int,int>> getRange(ChessISA, const Grid<ChessISA>&) const override;
};

class Bishop : public ChessPiece {
    std::vector<std::pair<int, int> > validMove (ChessISA instruct,const Grid<ChessISA>& grid) const override;
    public:
    Bishop(Group, PieceType);
    //std::vector<std::pair<int,int>> getRange(ChessISA, const Grid<ChessISA>&) const override;
};

class Queen : public ChessPiece {
    std::vector<std::pair<int, int> > validMove (ChessISA instruct,const Grid<ChessISA>& grid) const override;
    public:
    Queen(Group, PieceType);
    //std::vector<std::pair<int,int>> getRange(ChessISA, const Grid<ChessISA>&) const override;
};

class King : public ChessPiece {
    std::vector<std::pair<int, int> > validMove(ChessISA instruct,const Grid<ChessISA>& grid) const override;
    public:
    King(Group, PieceType);
    //std::vector<std::pair<int,int>> getRange(ChessISA, const Grid<ChessISA>&) const override;
};

class Knight : public ChessPiece {
    std::vector<std::pair<int, int> > validMove(ChessISA instruct,const Grid<ChessISA>& grid) const override;
    public:
    Knight(Group, PieceType);
    //std::vector<std::pair<int,int>> getRange(ChessISA, const Grid<ChessISA>&) const override;
};

class Pawn : public ChessPiece {
    std::vector<std::pair<int, int> > validMove(ChessISA instruct,const Grid<ChessISA>& grid) const override;
    public:
    Pawn(Group, PieceType);
    //std::vector<std::pair<int,int>> getRange(ChessISA, const Grid<ChessISA>&) const override;
};
#endif

