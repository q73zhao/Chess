
#include "chess_piece.h"
#include "grid.h"
#include "chess_instruction.h"
#include <cstdlib>
#include <algorithm>

using namespace std;
ChessISA ChessPiece::getState() const {
    return ChessISA{Instruction::State, group, pieceType};
}

ChessPiece::ChessPiece(Group g, PieceType pt) 
   : group{g}, pieceType{pt} {}


bool boundaryCheck(pair<int,int> p);
bool ChessPiece::noObstaclesAlongthePath(const ChessISA& instruct, const Grid<ChessISA>& grid) const {
	int srow = instruct.source.row;
	int scol = instruct.source.col;
	//std::cerr << instruct.source.row << " src " << instruct.source.col << std::endl;
	//std::cerr << instruct.destination.row << " dest " << instruct.destination.col << std::endl;
	const Piece<ChessISA>* temp = grid.getPiece(instruct.destination.row, instruct.destination.col);
	const Piece<ChessISA>* current = grid.getPiece(instruct.source.row, instruct.source.col);
	

	try{
	std::vector<Posn> inBetween = inBetweenPosns(instruct.source, instruct.destination);
	//std::cout << "inBetween.size(): " << inBetween.size() << std::endl;
	//std::cout << "inBetween[0]: " << inBetween[0] << std::endl;
	

    bool betweenEmpty = true;

    for(auto i: inBetween){
        if(grid.getPiece(i.row,i.col) != nullptr){
			//std::cout << "There is stuff in between!" << std::endl;
            betweenEmpty = false;
            break;
        }
    }

	if(current->getState().pieceType == PieceType::Pawn){
		
		if(temp == nullptr){
			return (instruct.source.col == instruct.destination.col);
		} else if(current->getState().group == temp->getState().group){
			return false;
		} else if(current->getState().group != temp->getState().group){
			return (instruct.source.col != instruct.destination.col);
		} else{
			return true;
		}

	}



    if(betweenEmpty && temp == nullptr){
		//std::cout << "There is no stuff in between and destination has a nullptr!" << std::endl;
	    return true;
    }else if(!betweenEmpty){
		//std::cout << "!betweenEmpty" << std::endl;
	    return false;
    } else if(grid.getPiece(srow,scol)->getState().group == temp->getState().group){
		//std::cout << "There is same group stuff in the destination!";
	    return false;
    } else {
	    return true;
    }
	} catch(const char * error) {
		if(temp == nullptr){
			return true;
		} else if(grid.getPiece(srow,scol)->getState().group == temp->getState().group){
		   return false;
		} else {
			return true;
		}
	}
}

std::vector<Posn> ChessPiece::inBetweenPosns(Posn source, Posn destination) const {
	std::vector<Posn> posnPool;
	// 
	//int rowDiff = destination.row - source.row;
	//int colDiff = destination.col - source.col;
	
	Posn difference = destination - source;
	//std::cout << "The differernce is " << difference << std::endl;
	// try {
	if (! 
	(
	(abs(difference.row) == abs(difference.col)) || 
	(difference.row == 0) ||
	(difference.col == 0)
	) 
	) {
		//std::cerr << "The posn is not valid! But I let you go for now!" << std::endl;
		throw "Not Valid Posn!";
	// }
	// catch (const char * error) {
		// std::cerr << error << endl;
	// }
	}
	Posn unitVector;
	if (difference.row == 0 && difference.col == 0) {
		//std::cerr << "No difference? What the fuck?";
		throw "No difference!";
	} else if (difference.row == 0) {
		unitVector = Posn{0, 
				difference.col / abs(difference.col)};
	} else if (difference.col == 0) {
		unitVector = Posn{difference.row / abs(difference.row), 
				0};
	} else {
		unitVector = Posn{difference.row / abs(difference.row), 
				difference.col / abs(difference.col)};
	}

	//std::cout << "Unit vector is " << unitVector << std::endl;
	
	Posn temp = source;
	while (temp != destination) {
		temp = temp + unitVector;
		//std::cout << "New temp vector is: " << temp << std::endl;
		posnPool.push_back(temp);
	}
	// the next grid is the destination 
	// so we stop one step before destination
	posnPool.pop_back();
	return posnPool;
}

bool ChessPiece::validate(const ChessISA& instruct, int currPlayerIndex, const Grid<ChessISA>& grid) const {


    const Piece<ChessISA>* tempPiece = grid.getPiece(instruct.source.row, instruct.source.col);

    if(Group(currPlayerIndex) == tempPiece->getState().group){
    
    std::vector<std::pair<int,int> > temp = this->validMove(instruct,grid);
    
    bool exist = false;
    
    for(auto i: temp){
        if(i.first == instruct.destination.row && i.second == instruct.destination.col){
			//std::cout << "Haha?" << std::endl;
            exist = true;
            break;
        }
    }
    
    if(exist){
        bool noObstacle = noObstaclesAlongthePath(instruct, grid);
        //std::cout << noObstacle << std::endl;
        return noObstacle;
        
    } else {
        return false;
    }

   return true;
    } else {
        return false;
    }
}

std::vector<std::pair<int,int>> ChessPiece::getRange(ChessISA instruct, const Grid<ChessISA>& grid) const {
    return advancedValidMove(instruct, grid);
}

// instruct gives the position of the current piece that we want to check
std::vector<std::pair<int, int>> ChessPiece::advancedValidMove(ChessISA instruct, const Grid<ChessISA>& grid) const {
	vector<pair<int,int>> init = validMove(instruct, grid);
	std::vector<pair<int,int>> temp;

	
	for (auto j : init) {
		if (boundaryCheck(j)) {
			temp.push_back(j);
		}
	}
	//std::vector<pair<int,int>> temp = this->validMove(instruct,grid);
    
	std::vector<pair<int, int>> possible;

    Posn source = instruct.source;
    
    const Piece<ChessISA>* current = grid.getPiece(instruct.source.row,instruct.source.col);


	for(auto i:temp){
        Posn tempdest{i.first,i.second};
		ChessISA newInstruct{Instruction::Moving,tempdest,source,current->getState().group,current->getState().pieceType};
		
		bool notDelete = validate(newInstruct,int(current->getState().group),grid);

		if(notDelete){
			//std::cout << i.first<< "," << i.second << std::endl;
			possible.push_back(i);
			continue;
		}
	}

	return possible;
}

Rook::Rook(Group g, PieceType pt) : ChessPiece{g, pt} {}
vector<pair<int,int>> Rook::validMove(ChessISA instruct, const Grid<ChessISA>& grid) const {
//std::cerr << "The function you are calling is Bishop::validMove. Please recall later." << std::endl;
    vector<pair<int,int>> rv;
    
	int row = instruct.source.row;
	int col = instruct.source.col;

	for(int i = 0; i < 8; i++){
        std::pair<int,int> move {row, i};

		if(i == col) {continue;}

		rv.push_back(move);
	}

	for(int j = 0; j < 8; j++){
        std::pair<int,int> move{j,col};
		
		if(j == row) {continue;}

		rv.push_back(move);
	}

    return rv;
}

Bishop::Bishop(Group g, PieceType pt) : ChessPiece{g, pt} {}
vector<pair<int,int>> Bishop::validMove(ChessISA instruct, const Grid<ChessISA>& grid) const {
vector<pair<int,int>> rv;

//std::cerr << "The function you are calling is Bishop::validMove. Please recall later." << std::endl;

int row = instruct.source.row;
int col = instruct.source.col;

int upperdiff = 7 - row;
int coldiff = col;
int startrow;
int startcol;

    if(upperdiff > coldiff) {
            startrow = row + coldiff;
            startcol = 0;
        } else {
            startrow = 7;
            startcol = col - upperdiff;
        }

        for(int i = 0; i < 8; i++){
                std::pair<int,int> move{startrow-i, startcol+i};
                if(startrow-i < 0 || startcol+i > 7) break;
                if(startrow-i == row && startcol+i == col) continue;
                rv.push_back(move);
    }


    int downdiff = row;
    coldiff = col;

    if(downdiff > coldiff) {
            startrow = row - coldiff;
            startcol = 0;
    } else {
            startrow = 0;
            startcol = col - downdiff;
    }
    for(int j = 0; j < 8; j++){
            std::pair<int,int> move{startrow+j, startcol+j};
            if(startrow+j > 7 || startcol +j > 7) break;
            if(startrow+j == row && startcol +j == col) continue;

            rv.push_back(move);
    }

return rv;
}

Queen::Queen(Group g, PieceType pt) : ChessPiece{g, pt} {}
vector<pair<int,int>> Queen::validMove(ChessISA instruct, const Grid<ChessISA>& grid) const {
vector<pair<int,int>> rv;
//std::cerr << "The function you are calling is Queen::validMove. Please recall later." << std::endl;
// Rook::getRange();
	int row = instruct.source.row;
	int col = instruct.source.col;

	for(int i = 0; i < 8; i++){
        std::pair<int,int> move {row, i};

		if(i == col) {continue;}

		rv.push_back(move);
	}

	for(int j = 0; j < 8; j++){
        std::pair<int,int> move{j,col};
		
		if(j == row) {continue;}

		rv.push_back(move);
	}

// Bishop::getRange();
row = instruct.source.row;
col = instruct.source.col;

int upperdiff = 7 - row;
int coldiff = col;
int startrow;
int startcol;

    if(upperdiff > coldiff) {
            startrow = row + coldiff;
            startcol = 0;
        } else {
            startrow = 7;
            startcol = col - upperdiff;
        }

        for(int i = 0; i < 8; i++){
                std::pair<int,int> move{startrow-i, startcol+i};
                if(startrow-i < 0 || startcol+i > 7) break;
                if(startrow-i == row && startcol+i == col) continue;
                rv.push_back(move);
    }


    int downdiff = row;
    coldiff = col;

    if(downdiff > coldiff) {
            startrow = row - coldiff;
            startcol = 0;
    } else {
            startrow = 0;
            startcol = col - downdiff;
    }
    for(int j = 0; j < 8; j++){
            std::pair<int,int> move{startrow+j, startcol+j};
            if(startrow+j > 7 || startcol +j > 7) break;
            if(startrow+j == row && startcol +j == col) continue;

            rv.push_back(move);
    }




return rv;
}

King::King(Group g, PieceType pt) : ChessPiece{g, pt} {}
vector<pair<int,int>> King::validMove(ChessISA instruct, const Grid<ChessISA>& grid) const {
    vector<pair<int,int>> rv;

//std::cerr << "The function you are calling is King::validMove. Please recall later." << std::endl;

int row = instruct.source.row;
int col = instruct.source.col;
int rowMax = std::min(7,instruct.source.row+1);
int colMin = std::max(0, instruct.source.col - 1);
int i_scale = rowMax - row +2;
int j_scale = col+1 - colMin+1;

for(int i = 0; i < i_scale; i++){
    if(rowMax-i == -1) break;
        for(int j = 0; j < j_scale; j ++){
            if(colMin+j == 8) break;

            std::pair<int,int> move{rowMax-i,colMin+j};
                if(move.first == instruct.source.row && move.second == instruct.source.col){
                    continue;
                    }
                    rv.push_back(move);               
                    }
            }
    return rv;
}

Knight::Knight(Group g, PieceType pt) : ChessPiece{g, pt} {}
vector<pair<int,int>> Knight::validMove(ChessISA instruct, const Grid<ChessISA>& grid) const {
    vector<pair<int,int>> rv;
	//std::cerr << "The function you are calling is Knight::validMove. Please recall later." << std::endl;
	

	int col = instruct.source.col;
	int row = instruct.source.row;
	int j;

	for(int i = -2; i < 3; i++) {
		if(i < 0){
			j = 3 + i;
		} else if(i > 0){
			j = 3 - i;
		} else {
			continue;
		}


		if(row+j <= 7 && col+i >= 0 && col+i <= 7) {
		std::pair<int,int> move1{row+j, col+i};
		rv.push_back(move1);
		}

		if(row-j >= 0 && col+i >=0 && col+i <= 7){
	    std::pair<int,int> move2{row-j, col+i};
		rv.push_back(move2);
		}
	}
    return rv;
}

Pawn::Pawn(Group g, PieceType pt) : ChessPiece{g, pt} {}
vector<pair<int,int>> Pawn::validMove(ChessISA instruct, const Grid<ChessISA>& grid) const {

	//std::cerr << "The function you are calling is Pawn::validMove. Please recall later." << std::endl;
    vector<pair<int,int>> rv;

    int scale = 1;
    int row = instruct.source.row;
	int col = instruct.source.col;
    
    const Piece<ChessISA>* current = grid.getPiece(instruct.source.row, instruct.source.col);

	if(current->getState().group == Group::Black){
		scale = -1;
	}
    
	//std::cerr << "isMoved?" << current->isMoved() <<std::endl;
	if(!current->isMoved()){
		//std::cout << "here" << std::endl;
		std::pair<int,int> move1{row+2*scale,col};
		//std::cout << move1.first << ","<<move1.second<<std::endl;
		rv.push_back(move1);
	}
 	std::pair<int,int> move2{row+scale,col};
	rv.push_back(move2);
	int newcol1 = col+1;
	int newcol2 = col-1;
	if(newcol1 < 8) {
		std::pair<int,int> move3{row+scale, newcol1};
		rv.push_back(move3);
	}

	if(newcol2 >= 0) {
		std::pair<int,int> move4{row+scale, newcol2};
		rv.push_back(move4);
	}

    return rv;
}

bool isInGrid(pair<int, int> src) {
    return (0 <= src.first) && (src.first < 8) && (0 <= src.second) && (src.second < 8); 
}

bool boundaryCheck(pair<int,int> p) {
	//std::cout <<p.first << p.second << std::endl;
	bool rv = true;
	rv = rv && p.first >= 0;
	rv = rv && p.second >= 0;
	rv = rv && p.first < 8;
	rv = rv && p.second < 8;
	return rv;
}

