#ifndef __GRAPHICSMonitor_H__
#define __GRAPHICSMonitor_H__

#include <iostream>
#include <vector>
#include "monitor.h"
#include "chess_instruction.h"
#include "grid.h"
#include "window.h"
class ChessPiece;
class ChessGraphicsMonitor : public Monitor<ChessISA> {
	Xwindow* window;
  std::vector<std::vector<const Piece<ChessISA>*>> currGrid;
  int phase = 10;
  int sideLength = 60;
  public:
  ChessGraphicsMonitor();
  ~ChessGraphicsMonitor();
  void init();
  void notify(const ChessISA& i , const Grid<ChessISA>& g) override;
  void monitor() override;
  void drawPiece(int x, int y, PieceType piece, Group group );
};

#endif

