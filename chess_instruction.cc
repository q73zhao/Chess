#include "chess_instruction.h"

std::string chessPieceString(Group grp, PieceType pt) {
    std::string rv;
    if (grp == Group::White) {
        rv = "White ";
    } else {
        rv = "Black ";
    }
    
    if (pt == PieceType::King) {
        rv += "King";
    } else if (pt == PieceType::Queen) {
        rv += "Queen";
    } else if (pt == PieceType::Rook) {
        rv += "Rook";
    } else if (pt == PieceType::Knight) {
        rv += "Knight";
    } else if (pt == PieceType::Bishop) {
        rv += "Bishop";
    } else {
        rv += "Pawn";
    }
    return rv;
}

std::string intToCoor(int row, int col) {
    std::string rv;
    rv.push_back('a' + col);
    rv +=  std::to_string(row + 1);
    return rv;
} 
std::string chessISAStr(const ChessISA&isa) {
    Instruction inst = isa.instruction; 
    Posn dest = isa.destination;
    Posn src = isa.source;
    Group group = isa.group;
    PieceType piece = isa.pieceType;
    
    std::string rv = "";
    if (inst == Instruction::NewPiece) {
        rv += "making a New Piece - ";
        rv += chessPieceString(group, piece);
        rv += " at " + intToCoor(dest.row, dest.col);

    } else if (inst == Instruction::Delate) {
        rv += "delete the Piece ";
        rv += " at " +  intToCoor(dest.row, dest.col);
    } else if (inst == Instruction::Undo) {
        rv += "undo your last instruction";
    } else if (inst == Instruction::Invalid) {
        rv += "Invalid";
    } else if (inst == Instruction::Stalemate) {
        rv += "Stalemate";
    } else if (inst == Instruction::Suggestion) {
        rv += "Suggestion";
    } else if (inst == Instruction::History) {
        rv += "History";
    } else if (inst == Instruction::Resign) {
        rv += "Resign";
    } else if (inst == Instruction::State) {
        rv += "Showing State";
    } else if (inst == Instruction::Moving) {
        rv += "moving Piece";
        rv += " at " + intToCoor(src.row, src.col);
        rv += " to " + intToCoor(dest.row, dest.col);
    }  else if (inst == Instruction::Castling) {
        rv += "castling with ";
        if (piece == PieceType::King) {
            rv += "Kingside Rook";
        } else if (piece == PieceType::Queen) {
            rv += "Queenside Rook";
        } else {
            rv += "Invalid";
        }
    } else if (inst == Instruction::EnPassant) {
        rv += "Enpassant Pawn by moving piece";
        rv += " at " + intToCoor(src.row, src.col);
        rv += " to " + intToCoor(dest.row, dest.col);
        rv += " and capture piece at " + intToCoor(src.row, dest.col);
    } else if (inst == Instruction::Promotion) {
        rv += "moving Piece";
        rv += " at " + intToCoor(src.row, src.col);
        rv += " to " + intToCoor(dest.row, dest.col);
        rv += " and promote to ";
        rv += chessPieceString(group, piece);
    }
    return rv;
}

Posn Posn::operator+(Posn other) {
    return Posn{row + other.row, col + other.col};
}

Posn operator*(int c, Posn p) {
    return Posn{c * p.row, c * p.col};
}

Posn Posn::operator*(int c) {
    return Posn{c * row, c * col};
}

std::ostream& operator<< (std::ostream & out, Posn posn) {
	out << "[" << posn.row << ", " << posn.col << "]";
	return out;
}
Posn Posn::operator-(Posn other) {
	return Posn{row - other.row, col - other.col};
}
bool Posn::operator!=(Posn other) {
	return ((row != other.row) || (col != other.col));
}

bool Posn::operator==(Posn other) {
	return ((row == other.row) and (col == other.col));
}

