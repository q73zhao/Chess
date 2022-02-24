#ifndef _CHESS_TEXTMonitor_H_
#define _CHESS_TEXTMonitor_H_
#include <iostream>
#include <vector>
#include "monitor.h"
#include "chess_instruction.h"
#include "grid.h"
class ChessTextMonitor: public Monitor<ChessISA> {
  std::vector<std::vector<char>> theMonitor;
  std::vector<char> theGrave;
  const int gridSize;
  public:
  ChessTextMonitor(int n);
  ~ChessTextMonitor();
  void monitor(); 
  void notify(const ChessISA&, const Grid<ChessISA>&) override;
};

#endif

