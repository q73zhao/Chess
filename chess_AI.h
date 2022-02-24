#ifndef _CHESS_AI_
#define _CHESS_AI_
#include "chess_instruction.h"
#include "AI.h"
#include "chess_piece.h"
#include <utility>
template <class InstructionSet> class Grid;
class ChessAI : public AI<ChessISA> {
    Grid<ChessISA>* tempGrid;
    std::vector<std::vector<std::vector<std::pair<int,int>>>> rangeGrid;
    std::vector<std::vector<std::vector<std::pair<int,int>>>> domainGrid;
    
    //std::vector<Posn> basicValidMove(const Piece<ChessISA>* pieces, Posn source);
    //std::vector<Posn> advancedValidMove(const Piece<ChessISA>* pieces, Posn source, const Grid<ChessISA>& grid);
   	//bool noObstaclesAlongthePath(const ChessISA& instruct, const Grid<ChessISA>& grid);
    //bool willCapture(const ChessISA& instruct, const Grid<ChessISA>& grid);
    //Posn kingPosn(int, const Grid<ChessISA>& grid);
	//bool willCheck(const ChessISA& instruct, const Grid<ChessISA>& grid);
    bool boundaryCheck(Posn p);

    bool inDanger(int row, int col, const Grid<ChessISA>&);
    bool KinginDanger(int currPlayer, const Grid<ChessISA>&grid);
    void flushTempGrid(const Grid<ChessISA>& grid);
    void flushRangeGrid(const Grid<ChessISA>& grid);
    void flushDomainGrid();
    ChessISA levelOne(int currPlayerIndex, const Grid<ChessISA>& grid);
    ChessISA levelTwo(int currPlayerIndex, const Grid<ChessISA>& grid);
    ChessISA levelThree(int currPlayerIndex, const Grid<ChessISA>& grid);

    public:
    ChessAI();
    ~ChessAI();
    ChessISA suggestion(int level, int currPlayerIndex, const Grid<ChessISA>&) override;
    bool validate(const ChessISA&, int currPlayerIndex, const Grid<ChessISA>&) override;
    // evaluate determines whether the game ends or not
    // If the game end, return true
    // If the game doesn't end, return false  

    // The function should handle these situations
    // 1. If one of the king is missing
    // 2. In stalemate. (no valid move but not in check)
    // Side effect: Updating the scoreboard.
    bool evaluate(const Grid<ChessISA>&, int currPlayerIndex, std::vector<float>&);

    bool setupCheck(const Grid<ChessISA>&);
    // To access piece, use grid.getPiece(int row, int col)
    //void updateAIGrid(const Grid<ChessISA>& grid) override;

};


#endif
