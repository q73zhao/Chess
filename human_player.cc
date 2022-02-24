#include "human_player.h"
#include "chess_instruction.h"
#include <iostream>
#include <sstream>
#include "piece.h"
#include "grid.h"

using namespace std;
static char getSymbol(const ChessISA&);

HumanPlayer::HumanPlayer(Group group, std::string name): Player<ChessISA>{name}, group{group} {
    
for (int i = 0; i < 8; i++) {
    std::vector<char> temp;
    for (int j = 0; j < 8; j++) {
      temp.push_back(' ') ;
    }
    theGrid.push_back(temp);
  }
}

HumanPlayer::HumanPlayer() : Player<ChessISA>{"name"}  {
    
for (int i = 0; i < 8; i++) {
    std::vector<char> temp;
    for (int j = 0; j < 8; j++) {
      temp.push_back(' ') ;
    }
    theGrid.push_back(temp);
  }
}

void HumanPlayer::notify(const ChessISA& instruct, const Grid<ChessISA>& grid) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            const Piece<ChessISA>* piece = grid.getPiece(i,j);
            if (piece) {
                theGrid[i][j] = getSymbol(piece -> getState());
            } else {
                theGrid[i][j] = ' ';
            }
        }
    }
}


ChessISA HumanPlayer::InstructionFetch() {
    argv.clear();
    argc = 0;
    cin >> command;
    string buffer;
    string argument;
    getline(std::cin, buffer);
    cin.clear();
    istringstream iss{buffer};
    //std::cerr << cin.fail() << std::endl;
    while (iss >> argument) {
        argv.push_back(argument);
        argc++;
    }
    ChessISA rv = InstructionDecodeCS246Mode();
    std::cout << chessISAStr(rv) << std::endl;
    ////std::cerr << "InstructFecth: command = " << command << " argc = "<< argc << std::endl;
    return rv;
}

ChessISA HumanPlayer::InstructionDecodeCS246Mode() {
    //std::cerr << "decode: CS246Mode 1\n";
    if (command == "resign") {
        //std::cerr << "decode: Resign \n";
        return ChessISA{Instruction::Resign};
    } else if (command == "undo") {
        //std::cerr << "decode: Undo \n";
        return ChessISA{Instruction::Undo};
    } else if (command == "stalemate") {
        return ChessISA{Instruction::Stalemate};
    } else if (command == "suggestion") {
        return ChessISA{Instruction::Suggestion};
    } else if (command == "history") {
        return ChessISA{Instruction::History};    
    } else if (argc < 2 or command != "move") {
        return ChessISA{Instruction::Invalid};
    }

    Posn  src{argv[0][1] - '1', argv[0][0] - 'a'};
    Posn dest{argv[1][1] - '1', argv[1][0] - 'a'};
    //std::cerr << "src{row:" << src.row << ", col:" << src.col <<"}\n" ;
    //std::cerr << "dest{row:" << dest.row << ", col:" << dest.col <<"}\n" ;
    //std::cerr << "decode: checkpoint 2\n";
    if (command != "move") {
        return ChessISA{Instruction::Invalid};
    }

    //std::cerr << "decode: checkpoint 3\n";
    if (argc == 2) {
        char srcPiece = theGrid[src.row][src.col];
        // Castling
        //std::cerr << "decode: checkpoint 3.5\n";
        if (srcPiece == 'K' or srcPiece == 'k') {
            //std::cerr << "decode: castling \n";
            // Castle with Kingside Rook
            if (dest.col - src.col > 1) {
                return ChessISA
                    {Instruction::Castling, group, PieceType::King};
            } else if (dest.col - src.col < -1) { 
                return ChessISA
                    {Instruction::Castling, group, PieceType::Queen};
            }
        } 
        // EnPassant
        //std::cerr << "decode: enpassant \n";
        if (srcPiece == 'P' or srcPiece == 'p') {
            if (dest.col != src.col && theGrid[dest.row][dest.col] == ' ') {
                return ChessISA{Instruction::EnPassant, dest, src};
            }
        }
        
        // Moving
        //std::cerr << "decode: moving \n";
         return ChessISA{Instruction::Moving, dest, src};
    }
    //std::cerr << "decode: checkpoint 4\n";
        // Promotion
    if (argc == 3) {
        Group premoteToGroup = group;
        PieceType premoteToPiece = PieceType::Pawn;
        char piece = argv[2][0];
        if ('a' <= piece and piece <= 'z') {
            premoteToGroup = Group::Black;
        } else {
            premoteToGroup = Group::White;
            piece = piece + 'a' - 'A';
        }
        switch(piece) {
        case 'q':
            premoteToPiece = PieceType::Queen;
            break;
        case 'n':
            premoteToPiece = PieceType::Knight;
            break;
        case 'b':
            premoteToPiece = PieceType::Bishop;
            break;
        case 'r':
            premoteToPiece = PieceType::Rook;
            break;
        default:
            return ChessISA
                {Instruction::Invalid, dest, src, premoteToGroup, premoteToPiece};
            break;
        }
        //std::cerr << "decode: promote \n";
        return ChessISA
            {Instruction::Promotion, dest, src, premoteToGroup, premoteToPiece};
    }
    //std::cerr << "decode: checkpoint 5\n";
    return ChessISA{Instruction::Invalid};
}

static char getSymbol(const ChessISA& state) {
    std::string symbols[] = {"Kk", "Qq", "Bb", "Rr", "Nn", "Pp"};
    return symbols[int(state.pieceType)][int(state.group)];
}

