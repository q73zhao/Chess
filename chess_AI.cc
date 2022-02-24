#include "chess_AI.h"
#include "grid.h"
#include "chess_piece.h"
#include "chess_instruction.h"
#include <vector>
#include <algorithm>

ChessAI::ChessAI() {
	tempGrid = new Grid<ChessISA>{8};
	std::vector<std::pair<int, int>> pairTemp;
	for (int i = 0; i < 8; i++) {
		std::vector<std::vector<std::pair<int,int>>> vectorPairTemp;
		for (int j = 0; j < 8; j++) {
			vectorPairTemp.push_back(pairTemp);
		}
		rangeGrid.push_back(vectorPairTemp);
		domainGrid.push_back(vectorPairTemp);
	}
}

ChessAI::~ChessAI() { delete tempGrid; }

void ChessAI::flushRangeGrid(const Grid<ChessISA>& grid) {
	//flushTempGrid(grid);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++)  {
			rangeGrid[i][j].clear();
		}
	}

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			const Piece<ChessISA>* temp = grid.getPiece(i,j);
			if (temp != nullptr) {
				rangeGrid[i][j] = temp -> getRange(ChessISA(Instruction::Moving,{i,j},{i,j}), grid);
					/*//std::cout << "Piece at row: " << i + 1<< " col: " << j + 1;
					if (rangeGrid[i][j].size()) {
						//std::cout << " has the folloing range\n";
					for (auto& vect: rangeGrid[i][j]) {
						//std:: cout << "row: " << vect.first + 1 << " col: " << vect.second + 1<< std::endl;
					}
					} else {
						//std::cout << " has no range\n"; */
							}
		}
		
	}
}

void ChessAI::flushDomainGrid() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			domainGrid[i][j].clear();
		}
	}

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			for (auto src: rangeGrid[i][j]) {
				domainGrid[src.first][src.second].push_back({i,j});
			}
		}
	}
}

void ChessAI::flushTempGrid(const Grid<ChessISA>& grid) {
	tempGrid -> copy(grid);
}


//std::vector<Posn> inBetweenPosns(Posn source, Posn destination);

bool  ChessAI::evaluate(const Grid<ChessISA>& grid, int currPlayerIndex, std::vector<float>& s) {
	int numKing[2] = {0};
	tempGrid -> copy(grid);
	Piece<ChessISA>* temp;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			//std::cerr << i << j << std::endl;
			temp = tempGrid -> getPiece(i,j);
			if (temp && temp -> getState().pieceType == PieceType::King) {
				numKing[int(tempGrid -> getPiece(i,j)  -> getState().group)]++;
			}
		}		
	}

	ChessISA sugg = suggestion(1,currPlayerIndex,grid);
	if (sugg.instruction == Instruction::Stalemate) {
		std::cout << "Stalemate!" << std::endl;
		s[0] += 0.5;
		s[1] += 0.5;
		return false;
	} else if (sugg.instruction == Instruction::Resign) {
		s[1 - currPlayerIndex]++;
		if (currPlayerIndex) {
			std::cout << "White Wins!" << std::endl;
		} else {
			std::cout << "Black Wins!" << std::endl;
		}
		return false;
	}

	if (numKing[0] == numKing[1]) {
		if (numKing[0] == 1) {
			return true;
		} else {
			std::cout << "Stalemate!" << std::endl;
			s[0] += 0.5;
			s[1] += 0.5;
		}
	} else {
		if (numKing[0] == 0) {
			s[1] += numKing[1];
			std::cout << "Black Wins!" << std::endl;
		} else if (numKing[1] == 0) {
			s[0] += numKing[0];
			std::cout << "White Wins!" << std::endl;
		}
	} 
	return false;
}

bool ChessAI::setupCheck(const Grid<ChessISA>& grid) {
	flushTempGrid(grid);
	flushRangeGrid(grid);
	flushDomainGrid();
	int numKing[2] = {0};
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			const Piece<ChessISA>* temp = grid.getPiece(i,j);
			if (temp && temp -> getState().pieceType == PieceType::King) {
				numKing[int(grid.getPiece(i,j) -> getState().group)]++;
				if (inDanger(i, j, grid)) {
					return false;
				}
			} else if (i == 0 || i == 7) {
				if (temp && temp -> getState().pieceType == PieceType::Pawn) {
					return false;
				}
			}
		}		
	}

	if (numKing[0] != 1 || numKing[1] != 1) {
		std::cout << "Num White King " << numKing[0] << std::endl;
		std::cout << "Num Black King " << numKing[1] << std::endl;
		return false;
	}
	return true;
}
ChessISA ChessAI::suggestion(int level, int currentPlayerIndex, const Grid<ChessISA>& grid) {
	switch (level) {
	case 1:
		return levelOne(currentPlayerIndex, grid);
	case 2:
		return levelTwo(currentPlayerIndex, grid);
	case 3:
		return levelThree(currentPlayerIndex, grid);
	default:
		return ChessISA{Instruction::Resign};
	}
}

bool ChessAI::validate(const ChessISA& instruct, int currPlayerIndex, const Grid<ChessISA>& grid) {
	int dest_row = instruct.destination.row;
    int dest_col = instruct.destination.col;
    int src_row = instruct.source.row;
    int src_col = instruct.source.col;
	


	int i;
	//Piece<ChessISA>* piece;
	std::vector<std::vector<std::pair<int,int>>> castleInfo = {
	//  rook, king 1, king2, king3, empty space
	{{7,7},{7,4},{7,6},{7,5}}, // Black King Side
	{{7,0},{7,4},{7,3},{7,2},{7,1}}, // Black Queen Side
	{{0,7},{0,4},{0,6},{0,5}}, // White King Side
	{{0,0},{0,4},{0,3},{0,2},{0,1}} // White Queen Side
	}; 

	switch (instruct.instruction) {
	case Instruction::Undo:
		return true;
	case Instruction::Resign:
		return true;
	case Instruction::Stalemate:
		return true;
	case Instruction::Suggestion:
		return true;
	case Instruction::Moving:
		flushTempGrid(grid);
		if (!(boundaryCheck(instruct.source) && boundaryCheck(instruct.destination))) { return false; }
		flushRangeGrid(grid);

		for (auto dest: rangeGrid[src_row][src_col]){
			if (dest.first == dest_row && dest.second == dest_col) { // in range
				tempGrid -> set(tempGrid -> withdrawal(src_row, src_col), dest_row, dest_col);
				flushRangeGrid(*tempGrid);
				if (KinginDanger(currPlayerIndex, *tempGrid)) {
					return false;
				} else {
					return true;
				}
			}
		}
		return false;
	case Instruction::Castling:

		flushTempGrid(grid);
		flushRangeGrid(grid);

		
		i = (currPlayerIndex == int(Group::Black))? 0 : 2;
		i += (instruct.pieceType == PieceType::King)? 0 : 1;

		
		//std::cout << "ckp1 " << i <<std::endl;
		// king rook if moved
		//std:: cerr << grid.getPiece(castleInfo[i][0].first, castleInfo[i][0].second) << grid.getPiece(castleInfo[i][1].first, castleInfo[i][1].second) << std::endl;
		if (grid.getPiece(castleInfo[i][0].first, castleInfo[i][0].second) -> isMoved() ||
			grid.getPiece(castleInfo[i][1].first, castleInfo[i][1].second) -> isMoved()) {
				return false;
		}
		
		// obstacle
		for (unsigned int j = 2; j < castleInfo[i].size(); j++ ) {
			//std::cout << "ckp2" <<std::endl;
			if ((grid.getPiece(castleInfo[i][j].first, castleInfo[i][j].second) != nullptr)) {
			return false;
			}
		}
		

		for (int j = 1; j < 4; j++) {
			//std::cout << "ckp3" <<std::endl;
			if (inDanger(castleInfo[i][j].first, castleInfo[i][j].second, grid)) {
				//std::cout << "in danger" <<  j << std::endl; 
				return false;
			}
		}
		//std::cout << "true" <<std::endl;
		return true;

	case Instruction::EnPassant:
		flushTempGrid(grid);
		flushRangeGrid(grid);
		if (currPlayerIndex == 1 && src_row  != 3 ) {
			return false;
		} else if(currPlayerIndex == 0 && src_row != 4){
			return false;
		} else if (dest_col - src_col == 1 || dest_col - src_col == -1) { // col check
			if (dest_row - src_row == (currPlayerIndex? -1: 1)) {
				if (KinginDanger(currPlayerIndex, *tempGrid)) {
					return false;
				}
				return true;
			}
		}
		return false;
	case Instruction::Promotion:
		// a promotion must be a valid move.
		if (!validate(ChessISA{Instruction::Moving, instruct.destination, instruct.source}, currPlayerIndex, grid)) {
			return false;
		} else if ((dest_row == 7 && currPlayerIndex == 0) && (dest_row == 0 && currPlayerIndex == 1)) {
			return true;
		}
		break;
	case Instruction::History:
		return true;
	default:
		return false;
	}

	return true;
}


bool ChessAI::inDanger(int row, int col, const Grid<ChessISA>& grid) {
	Group myGroup = Group(0);
	if (grid.getPiece(row, col)) {
		myGroup = grid.getPiece(row, col) -> getState().group;
	}
	
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			Piece<ChessISA> * temp = grid.getPiece(i, j);
			if (temp && temp ->getState().group != myGroup) {
				for (auto vecRange: rangeGrid[i][j]) {
					if (vecRange.first == row && vecRange.second == col) {
					return true;
					}
				}
			}	
		}	
	}
	return false;
}

bool ChessAI::KinginDanger(int currPlayer, const Grid<ChessISA>&grid) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			const Piece<ChessISA>* temp = grid.getPiece(i,j);
			if (temp && temp -> getState().pieceType == PieceType::King) {
				if (temp -> getState().group == Group(currPlayer) && inDanger(i, j, grid)) {
					return true;
				}
			}
		}
	}
	return false;
}

bool ChessAI::boundaryCheck(Posn p) {
	bool rv = true;
	rv = rv && p.row >= 0;
	rv = rv && p.col >= 0;
	rv = rv && p.row < 8;
	rv = rv && p.col < 8;
	return rv;
}

ChessISA ChessAI::levelOne(int currPlayerIndex, const Grid<ChessISA>& grid) {
	//std::cout<< "level one was called:" << std::endl;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			Piece<ChessISA> * temp = grid.getPiece(i,j);
			if (temp && temp -> getState().group == Group(currPlayerIndex)) {
				flushRangeGrid(grid);
				flushTempGrid(grid);
				
				unsigned int rangeSize = rangeGrid[i][j].size();
				//std::cout << "rangesize: "<< rangeSize << std::endl;
				for (unsigned int k = 0; k < rangeSize; k += 1) {
					//std::cout << "K is " << k;
					auto dest = rangeGrid[i][j][k];
					//std::cout<< "level one :" << i << j;
					//std::cout<< "dest:" << dest.first << dest.second <<std::endl;
					ChessISA newISA {Instruction::Moving,Posn{dest.first, dest.second},	Posn{i,j}};
					if (validate(newISA, currPlayerIndex, grid)) {
						//std::cout << "return newISA\n";
						return newISA;
					}
				}
			}
		}
	}
	//std::cout << "level one return resign\n";
	flushRangeGrid(grid);
	flushTempGrid(grid);
	if (KinginDanger(currPlayerIndex, grid)) {
		return ChessISA{Instruction::Resign};
	} else {
		return ChessISA{Instruction::Stalemate};
	}
	
}

ChessISA ChessAI::levelTwo(int currPlayerIndex, const Grid<ChessISA>& grid) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			Piece<ChessISA> * temp = grid.getPiece(i,j);
			if (temp && temp -> getState().group == Group(1 - currPlayerIndex)) { // enemy pieces
				flushRangeGrid(grid);
				flushTempGrid(grid);
				flushDomainGrid();
				for (auto dest: domainGrid[i][j]) {
					ChessISA newISA {Instruction::Moving, Posn{i,j}, Posn{dest.first,dest.second}};
					if (validate(newISA, currPlayerIndex, grid)) {
						return newISA;
					}
				}
			}
		}
	}
	return levelOne(currPlayerIndex, grid);
}

ChessISA ChessAI::levelThree(int currPlayerIndex, const Grid<ChessISA>& grid) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			Piece<ChessISA> * temp = grid.getPiece(i,j);
			if (temp && temp -> getState().group == Group(currPlayerIndex)) { // enemy pieces
				flushRangeGrid(grid);
				flushTempGrid(grid);
				flushDomainGrid();
				for (auto dest: domainGrid[i][j]) {
					if (domainGrid[i][j].size() == 0) {
						continue;
					}
					ChessISA newISA {Instruction::Moving, Posn{dest.first,dest.second}, Posn{i,j}};
					if (validate(newISA, currPlayerIndex, grid)) {
						return newISA;
					}
				}
			}
		}
	}
	return levelOne(currPlayerIndex, grid);
}
