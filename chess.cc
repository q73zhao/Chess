#include "game.h"
#include "chess.h"
#include "chess_instruction.h"
#include "grid.h"
#include "AI.h"
#include "chess_AI.h"
#include "piece_creator.h"
#include "chess_piece_creator.h"
#include "piece.h"
#include <vector>

// constructor
Chess::Chess() : Game<ChessISA>{new Grid<ChessISA>{8}, new ChessPieceCreator, new ChessAI, 2} {
    for (int i = 0; i < 8; i++) {
      std::vector<Piece<ChessISA>*> temp;
      for (int j = 0; j < 8; j++) {
        temp.push_back(nullptr);
      }
    setupGrid.push_back(temp);
  }
}

// destructor
Chess::~Chess() {
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        delete setupGrid[i][j];
      }
  }
}


void Chess::movingExecute(const ChessISA& instruct) {
    int dest_row = instruct.destination.row;
    int dest_col = instruct.destination.col;
    int src_row = instruct.source.row;
    int src_col = instruct.source.col;
    bool ifKill = false;

    Piece<ChessISA>* destPiece = grid -> withdrawal(dest_row, dest_col);
    if (destPiece) { 
        if (destPiece -> getState().pieceType == PieceType::King) {
            std::cout << "Checkmate! ";
            if (destPiece -> getState().group == Group::Black) {
                std::cout << "WhiteWin!\n"; 
            } else {
                std::cout << "BlackWin!\n"; 
            }
        }
        grid -> attachToGrave(destPiece);
        ifKill = true;
    }
    Piece<ChessISA>* sourcePiece = grid -> withdrawal(src_row, src_col);
    sourcePiece -> increaseNumMoved();
    grid -> set(sourcePiece, dest_row, dest_col);
    grid -> set(nullptr, src_row, src_col);
    instructionHistory.push_back({instruct, ifKill});
}

void Chess::castlingExecute(const ChessISA& instruct) {
    Piece<ChessISA>* piece = nullptr;
    std::vector<std::vector<std::pair<int,int>>> castleInfo = {
        {{7,4},{7,6},{7,7},{7,5}},  // Black King Side
        {{7,0},{7,3},{7,4},{7,2}},  // Black Queen Side
        {{0,4},{0,6},{0,7},{0,5}},  // White King Side
        {{0,0},{0,3},{0,4},{0,2}}   // White Queen Side
    }; 
    int i = (Group(currPlayer) == Group::Black)? 0 : 2;
    i += (instruct.pieceType == PieceType::King)? 0 : 1;

    piece = grid -> withdrawal(castleInfo[i][0].first,castleInfo[i][0].second);
    piece -> increaseNumMoved();
    grid -> set(piece, castleInfo[i][1].first,castleInfo[i][1].second);

    piece = grid -> withdrawal(castleInfo[i][2].first,castleInfo[i][2].second);
    piece -> increaseNumMoved();
    grid -> set(piece, castleInfo[i][3].first,castleInfo[i][3].second);

    grid -> set(nullptr, castleInfo[i][0].first,castleInfo[i][0].second);
    grid -> set(nullptr, castleInfo[i][2].first,castleInfo[i][2].second);
    instructionHistory.push_back({instruct, false});
}

void Chess::enPassantExecute(const ChessISA& instruct) {
    int dest_row = instruct.destination.row;
    int dest_col = instruct.destination.col;
    int src_row = instruct.source.row;
    int src_col = instruct.source.col;
    Piece<ChessISA>* destPiece = grid -> withdrawal(src_row, dest_col);

    if (destPiece) { grid -> attachToGrave(destPiece); }

    Piece<ChessISA>* piece = grid -> withdrawal(src_row, src_col);
    piece -> increaseNumMoved();
    grid -> set(piece, dest_row, dest_col);
    grid -> set(nullptr, src_row, src_col);

    instructionHistory.push_back({instruct, true});
}

void Chess::promotionExecute(const ChessISA& instruct) {
    movingExecute(instruct);
    int dest_row = instruct.destination.row;
    int dest_col = instruct.destination.col;
    grid -> attachToGrave(grid -> withdrawal(dest_row, dest_col));
    grid -> set(pieceCreator -> generate(instruct), dest_row, dest_col);
}

void Chess::resignExecute(const ChessISA& instruct) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            const Piece<ChessISA>* curr = grid -> getPiece(i, j);
            if (curr && curr -> getState().pieceType == PieceType::King) {
                if (curr -> getState().group == Group(currPlayer)){
                    delete grid -> withdrawal(i,j);
                    grid -> set(nullptr, i, j);
                }
            }
        }
    }
}

void Chess::stalemateExecute(const ChessISA& instruct) {
    if (stalemateRequest) {
        if (instruct.instruction == Instruction::Stalemate) {
            resignExecute(instruct);
            currPlayer = 1 - currPlayer;
            resignExecute(instruct);       
        } else {
            stalemateRequest = false;    
        }
    } else {
        stalemateRequest = true;
    }

}

void Chess::historyExecute() {
    for (auto instruc: instructionHistory) {
        std::cout << chessISAStr(instruc.first) << std::endl;
    }

}

void Chess::undoExecute() {
    ChessISA instruct = instructionHistory.back().first;
    switch (instruct.instruction) {
    case Instruction::Moving:
        reverseMovingExecute(instruct, instructionHistory.back().second);
        break;
    case Instruction::Castling:
        reverseCastlingExecute(instruct);
        break;
    case Instruction::EnPassant:
        reverseEnPassantExecute(instruct);  
        break;
    case Instruction::Promotion:
        reversePromotionExecute(instruct, instructionHistory.back().second); 
        break;
    default:
        break;
    }
}


bool Chess::execute(const ChessISA& instruct, bool validationCheck) {
    if (validationCheck)  {
        if ((ai -> validate(instruct, currPlayer, *grid) == false)) {
            std::cout << "Player " << currPlayer << " send an Invalid Input\n";
            return false;
        }
    }

    ChessISA tempISA{Instruction::Invalid};
    bool judge = true;
    int gap;

    switch (instruct.instruction) {
    case Instruction::NewPiece:
        grid -> set(pieceCreator -> generate(instruct), instruct.destination.row, instruct.destination.col);
        break;
    case Instruction::Resign:
        resignExecute(instruct);
        currPlayer++;
        break;
    case Instruction::Suggestion:
        std::cout << chessISAStr(ai -> suggestion(1,currPlayer, *grid)) << std::endl;
        break;
    case Instruction::Moving:
        movingExecute(instruct);
        currPlayer++;
        break;
    case Instruction::Castling:
        castlingExecute(instruct);
        currPlayer++;
        break;
    case Instruction::EnPassant:
        if (instructionHistory.size() < 1) {
            std::cout<< "Invalid" << std::endl;
            return false;
        } else {
            tempISA = instructionHistory.back().first;
            
            judge = judge && (tempISA.instruction == Instruction::Moving);
            //std::cout << "cp1 " << judge << std::endl;
            gap = tempISA.destination.row - tempISA.source.row;
            judge = judge && (gap*gap == 4);
            //std::cout << "cp2 " << judge << std::endl;
            judge = judge && (tempISA.destination.col == tempISA.source.col) && (tempISA.source.col == instruct.destination.col) ;
            //std::cout << "cp3 " << judge << std::endl;
            Piece<ChessISA>* piece = grid -> getPiece(tempISA.destination.row, tempISA.destination.col);
            //std::cout << "dest row " << instruct.destination.row << " dest col " << instruct.destination.col << std::endl;  
            //std::cout << "pinter " << piece << std::endl;
            judge = judge && piece && (piece -> getState().pieceType == PieceType::Pawn);
            //std::cout << "cp5 " << judge << std::endl;
            if (!judge) {return false;}
            enPassantExecute(instruct);
            currPlayer++;  
        }
        break;
    case Instruction::Promotion:
        promotionExecute(instruct); 
        currPlayer++;
        break;
    case Instruction::Undo:
        if (instructionHistory.size() == 1) {
            undoExecute();
            instructionHistory.pop_back();
        } else if (instructionHistory.size() > 1) {
            undoExecute();
            instructionHistory.pop_back();
            undoExecute();
            instructionHistory.pop_back();
            currPlayer++;
        }
        break;

    case Instruction::Stalemate:
        stalemateExecute(instruct);
        currPlayer++;
        break;
    
    case Instruction::History:
        historyExecute();
        break;

    default:
        break;
    }

    currPlayer = currPlayer % maxPlayer;
    grid -> notifyObservers(instruct);
    if (playersLevel[currPlayer] >= 0) {
        if (stalemateRequest) {
            execute(ChessISA{Instruction::Stalemate});
        } else {
        execute(ai -> suggestion(playersLevel[currPlayer],currPlayer,*grid), false);
        }
    }
    return true;
}

void Chess::init() {
    Instruction newPiece = Instruction::NewPiece;
    ChessISA temp{Instruction::Invalid};
    if (defaultSetup) {
        std::vector<PieceType> pieces = { 
            PieceType::Rook, 
            PieceType::Knight,
            PieceType::Bishop,
            PieceType::Queen,
            PieceType::King,
            PieceType::Bishop,
            PieceType::Knight,
            PieceType::Rook,
            };

        Group group = Group::White;
        for (int i = 0; i < 8; i++) {
            temp = ChessISA{newPiece, {0, i}, group, pieces[i]};
            grid -> set(pieceCreator -> generate(temp),0 ,i);
        
            temp = ChessISA{newPiece, {1, i}, group, PieceType::Pawn};
            grid -> set(pieceCreator -> generate(temp),1 ,i);
        }
        group = Group::Black;;
        for (int i = 0; i < 8; i++) {
            temp = ChessISA{newPiece, {7, i}, group, pieces[i]};
            grid -> set(pieceCreator -> generate(temp),7 ,i);
            temp = ChessISA{newPiece, {6, i}, group, PieceType::Pawn};
            grid -> set(pieceCreator -> generate(temp),6 ,i);
            
        }
    } else {
         for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (setupGrid[i][j]) {
                    temp = setupGrid[i][j] -> getState();
                    grid -> set(pieceCreator -> generate(ChessISA{newPiece, temp.group, temp.pieceType}), i ,j);
                }
            }
        }
    }
    grid -> notifyObservers(temp);
    flushMonitor();
}


bool Chess::isFinish() {
    return !ai -> evaluate(*grid, currPlayer, playerScore);
}


void Chess::setup() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            grid -> set(setupGrid[i][j], i, j);
        }
    }
    grid -> notifyObservers(ChessISA{Instruction::Invalid});
    flushMonitor();
    std::string cmd, arg1, arg2;
    while (std::cin >> cmd) {
        
        if (cmd == "+") {
            std::cin >> arg1 >> arg2; 
            char piece = arg1[0];

            Group group;
            PieceType p;
            if ('a' <= piece and piece <= 'z') {
                group = Group::Black;
            } else {
                group = Group::White;
                piece = piece + 'a' - 'A';
            }
            switch(piece) {
            case 'q':
                p = PieceType::Queen;
                break;
            case 'n':
                p = PieceType::Knight;
                break;
            case 'b':
                p = PieceType::Bishop;
                break;
            case 'r':
                p = PieceType::Rook;
                break;
            case 'p':
                p = PieceType::Pawn;
                break;
            case 'k':
                p = PieceType::King;
                break;
            default:
                break;
            }
            int row = arg2[1] - '1';
            int col = arg2[0] - 'a';
            std::cerr << row << " " << col << "\n";

            delete grid -> withdrawal(row, col);
            grid -> set(pieceCreator -> generate(ChessISA{Instruction::NewPiece, group, p}),row ,col);

        } else if (cmd == "-") {
            std::cin >> arg1;
            int row = arg1[1] - '1';
            int col = arg1[0] - 'a';
            delete grid -> withdrawal(row, col);
            grid -> set(nullptr, row ,col);
        } else if (cmd == "=") {
            std::cin >> arg1;
            if (arg1 == "black") {
                currPlayer = 1;
            } else {
                currPlayer = 0;
            }
        } else if (cmd == "done") {
            if (ai -> setupCheck(*grid)) {
                std::cout << "Valid Setup\n";
                for (int i = 0; i < 8; i++) {
                    for (int j = 0; j < 8; j++) {
                        setupGrid[i][j] = grid -> withdrawal(i, j);
                    }
                }
                defaultSetup = false;
                return;
            } else {
                std::cout << "Invalid Setup\n";
                continue;
            }
        }
        grid -> notifyObservers(ChessISA{Instruction::Invalid});
        flushMonitor();
    }
}

void Chess::clear() {
    grid -> clear();
    instructionHistory.clear();
    stalemateRequest = false;
}

void Chess::reverseCastlingExecute(const ChessISA& instruct) {
    Piece<ChessISA>* piece = nullptr;
    std::vector<std::vector<std::pair<int,int>>> castleInfo = {
        {{7,4},{7,6},{7,7},{7,5}}, // Black King Side
        {{7,0},{7,3},{7,4},{7,2}}, // Black Queen Side
        {{0,4},{0,6},{0,7},{0,5}}, // White King Side
        {{0,4},{0,6},{0,7},{0,5}} // White Queen Side
    }; 
    int i = (Group(currPlayer) == Group::Black)? 0 : 2;
    i += (instruct.pieceType == PieceType::King)? 0 : 1;

    piece = grid -> withdrawal(castleInfo[i][1].first,castleInfo[i][1].second);
    piece -> decreaseNumMoved();
    grid -> set(piece, castleInfo[i][0].first,castleInfo[i][0].second);

    piece = grid -> withdrawal(castleInfo[i][3].first,castleInfo[i][3].second);
    piece -> decreaseNumMoved();
    grid -> set(piece, castleInfo[i][2].first,castleInfo[i][2].second);

    grid -> set(nullptr, castleInfo[i][1].first,castleInfo[i][1].second);
    grid -> set(nullptr, castleInfo[i][3].first,castleInfo[i][3].second);
}

void Chess::reverseEnPassantExecute(const ChessISA& instruct) {
    int dest_row = instruct.destination.row;
    int dest_col = instruct.destination.col;
    int src_row = instruct.source.row;
    int src_col = instruct.source.col;

    Piece<ChessISA>* destPiece = grid -> detachFromGrave();
    grid -> set(destPiece, src_row, dest_col);

    Piece<ChessISA>* piece = grid -> withdrawal(dest_row, dest_col);
    piece -> decreaseNumMoved();
    grid -> set(nullptr, dest_row, dest_col);
    grid -> set(piece, src_row, src_col);
}
void Chess::reversePromotionExecute(const ChessISA& instruct, bool ifKill) {
    int dest_row = instruct.destination.row;
    int dest_col = instruct.destination.col;
    Piece<ChessISA>* oldPiece = grid -> detachFromGrave();
    delete grid -> withdrawal(dest_row, dest_col);
    grid -> set(oldPiece, dest_row, dest_col);
    reverseMovingExecute(instruct, ifKill);

}
void Chess::reverseMovingExecute(const ChessISA& instruct, bool ifKill) {
    int dest_row = instruct.destination.row;
    int dest_col = instruct.destination.col;
    int src_row = instruct.source.row;
    int src_col = instruct.source.col;

    Piece<ChessISA>* sourcePiece = grid -> withdrawal(dest_row, dest_col);
    sourcePiece -> decreaseNumMoved();
    grid -> set(sourcePiece, src_row, src_col);


    Piece<ChessISA>* destPiece = ifKill? grid -> detachFromGrave() : nullptr;
    grid -> set(destPiece, dest_row, dest_col);
    
}

