#ifndef _CHESS_INSTRUCTION_
#define _CHESS_INSTRUCTION_
#include <iostream>
#include <string>
enum class Instruction { NewPiece, Delate, Undo, Invalid, Resign, Stalemate, State, Suggestion,
                         Moving, Castling, EnPassant, Promotion, History};
/* Instructions are:
    NewPiece -- Set a New Piece
        {NewPicece, Group, PieceType}

    Moving -- Move piece from posn 1 to posn 2.
        {Moving, Posn destination, Posn source}

    Castling -- Castling
        {Castling, Group, PieceType} 
            PieceType is either King(castle with Kingside Rook) 
            or Queen(castle with Queenside Rook).

    EnPassant -- EnPassant
        {Enpassant, Posn destination, Posn source}
    
    Promotion -- Promotion
        {Promotion, Posn dest, Posn source, Group, pieceType}

    Resign -- Resign
        {Resign}
*/
enum class PieceType { King, Queen, Bishop, Rook, Knight, Pawn };
enum class Group { White, Black };

struct Posn {
    int row;
    int col;
    Posn operator+(Posn other);
    Posn operator*(int);
    Posn operator-(Posn other);
	bool operator!=(Posn other);
	friend std::ostream& operator<< (std::ostream & out, Posn posn);
	friend Posn operator*(int c, Posn p);
    bool operator==(Posn other);
};
Posn operator*(int, Posn);

struct ChessISA {
    Instruction instruction; 
    Posn destination, source;
    Group group;
    PieceType pieceType;
    // Construction used for NewPiece
    //  {NewPicece, Group, PieceType}
 
    ChessISA(Instruction inst, Posn dest, Group group, PieceType pt) 
        : instruction{inst}, destination{dest}, group{group}, pieceType{pt} {}
    // Construction used for Castling
    //  {Castling, Group, PieceType} 
    ChessISA(Instruction inst, Group group, PieceType pt) 
        : instruction{inst}, group{group}, pieceType{pt} {}

    // Constructor used for Moving and Castling
    //  {Moving, Posn destination, Posn source}
    //  {Enpassant, Posn destination, Posn source}
    ChessISA(Instruction inst, Posn dest, Posn src) 
        : instruction{inst}, destination{dest}, source{src} {}

    // Constructor used for Resign
    //  {Resign}
    ChessISA(Instruction inst) 
        :  instruction{inst}, group{group} {}
    // Constructor used for Promotion
    //  {Promotion, Posn dest, Posn source, Group, pieceType}
    ChessISA(Instruction inst, Posn dest, Posn src, Group group, PieceType piece) 
        : instruction{inst}, destination{dest}, source{src}, group{group}, pieceType{piece} {}
};

std::string chessPieceString(Group grp, PieceType pt);

std::string chessISAStr(const ChessISA&isa);
#endif

