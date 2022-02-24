#include "chess_graphics_monitor.h"
#include "chess_instruction.h"
#include "piece.h"
#include "grid.h"
#include <iostream>


ChessGraphicsMonitor::ChessGraphicsMonitor() {
  window = new Xwindow;
  init();
  for (int i = 0; i < 8; i++) {
    std::vector<const Piece<ChessISA>*> temp;
    for (int j = 0; j < 8; j++) {
      temp.push_back(nullptr);
    }
    currGrid.push_back(temp);
  }
}




void ChessGraphicsMonitor::init() {
  window -> fillRectangle(0,0,500,500, Xwindow::Black);
  window -> fillRectangle(phase,phase,480,480, Xwindow::DarkGreen);
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if ((7 - i + j) % 2) {
      window -> fillRectangle(j * sideLength + phase, i * sideLength + phase, sideLength, sideLength, Xwindow::Orange);
      }
    }
  }
}

ChessGraphicsMonitor::~ChessGraphicsMonitor() {
  delete window;
}



void ChessGraphicsMonitor::notify(const ChessISA& i , const Grid<ChessISA>& g ) {
  const Piece<ChessISA>* temp;
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      temp = g.getPiece(row, col);
      if (temp != currGrid[row][col]) {
        window -> fillRectangle(col * sideLength + phase,
                                (7 - row) * sideLength + phase,
                                sideLength, sideLength, 
                                (row + col) % 2? Xwindow::Orange : Xwindow::DarkGreen);
        if (temp) {
          drawPiece(col, 7 - row, temp -> getState().pieceType, temp -> getState().group);
        }
        currGrid[row][col] = temp;
      }
    }
  }

}

void ChessGraphicsMonitor::monitor() {
  return ;
}

void ChessGraphicsMonitor::drawPiece(int x, int y, PieceType piece, Group group) {
  int colour = (group == Group::White)? Xwindow::White :Xwindow::Black;
  int bkColour = (7 - x + y) % 2? Xwindow::Orange : Xwindow::DarkGreen;
  int merge;
  switch (piece)
  {
  case PieceType::King:
    merge = 10;
    window -> fillRectangle(phase + x * sideLength + merge, phase + y * sideLength + merge , sideLength - merge * 2, sideLength - merge * 2, colour);
    merge = 15;
    window -> fillRectangle(phase + x * sideLength + merge, phase + y * sideLength + merge , sideLength - merge * 2, sideLength - merge * 2, bkColour);
    merge = 20;
    window -> fillRectangle(phase + x * sideLength + merge, phase + y * sideLength + merge , sideLength - merge * 2, sideLength - merge * 2, colour);
    merge = 25;
    window -> fillRectangle(phase + x * sideLength + merge, phase + y * sideLength + merge , sideLength - merge * 2, sideLength - merge * 2, bkColour);
    break;
  
  case PieceType::Pawn:
    if (group == Group::Black) {
      window -> fillRectangle(phase + x * sideLength + 26 , phase + y * sideLength + 42, 8, 8, colour);
      window -> fillRectangle(phase + x * sideLength + 18 , phase + y * sideLength + 42, 4, 4, colour);
      window -> fillRectangle(phase + x * sideLength + 38 , phase + y * sideLength + 42, 4, 4, colour);
    } else {
      window -> fillRectangle(phase + x * sideLength + 26 , phase + y * sideLength + 10, 8, 8, colour);
      window -> fillRectangle(phase + x * sideLength + 18 , phase + y * sideLength + 14, 4, 4, colour);
      window -> fillRectangle(phase + x * sideLength + 38 , phase + y * sideLength + 14, 4, 4, colour);
    }
    merge = 22;
    window -> fillRectangle(phase + x * sideLength + merge, phase + y * sideLength + merge , sideLength - merge * 2, sideLength - merge * 2, colour);
    merge = 25;
    window -> fillRectangle(phase + x * sideLength + merge, phase + y * sideLength + merge , sideLength - merge * 2, sideLength - merge * 2, bkColour);
    break;
  case PieceType::Rook:

    window -> fillRectangle(phase + x * sideLength + 4 , phase + y * sideLength + 26, 52, 8, colour);
    window -> fillRectangle(phase + x * sideLength + 26 , phase + y * sideLength + 4, 8, 52, colour);

    merge = 19;
    window -> fillRectangle(phase + x * sideLength + merge, phase + y * sideLength + merge , sideLength - merge * 2, sideLength - merge * 2, bkColour);
    merge = 22;
    window -> fillRectangle(phase + x * sideLength + merge, phase + y * sideLength + merge , sideLength - merge * 2, sideLength - merge * 2, colour);
    merge = 25;
    window -> fillRectangle(phase + x * sideLength + merge, phase + y * sideLength + merge , sideLength - merge * 2, sideLength - merge * 2, bkColour);
    break;
  case PieceType::Knight:
    window -> fillRectangle(phase + x * sideLength + 13 , phase + y * sideLength + 4, 8, 8, colour);
    window -> fillRectangle(phase + x * sideLength + 4 , phase + y * sideLength + 13, 8, 8, colour);
    window -> fillRectangle(phase + x * sideLength + 39 , phase + y * sideLength + 4, 8, 8, colour);
    window -> fillRectangle(phase + x * sideLength + 4 , phase + y * sideLength + 39, 8, 8, colour);
    window -> fillRectangle(phase + x * sideLength + 13 , phase + y * sideLength + 47, 8, 8, colour);
    window -> fillRectangle(phase + x * sideLength + 47 , phase + y * sideLength + 13, 8, 8, colour);
    window -> fillRectangle(phase + x * sideLength + 39 , phase + y * sideLength + 47, 8, 8, colour);
    window -> fillRectangle(phase + x * sideLength + 47 , phase + y * sideLength + 39, 8, 8, colour);

    merge = 22;
    window -> fillRectangle(phase + x * sideLength + merge, phase + y * sideLength + merge , sideLength - merge * 2, sideLength - merge * 2, colour);
    merge = 25;
    window -> fillRectangle(phase + x * sideLength + merge, phase + y * sideLength + merge , sideLength - merge * 2, sideLength - merge * 2, bkColour);
    break;

  case PieceType::Bishop:
    window -> fillRectangle(phase + x * sideLength + 4  , phase + y * sideLength + 4, 8, 8, colour);
    window -> fillRectangle(phase + x * sideLength + 12 , phase + y * sideLength + 12, 8, 8, colour);
    window -> fillRectangle(phase + x * sideLength + 40 , phase + y * sideLength + 40, 8, 8, colour);
    window -> fillRectangle(phase + x * sideLength + 48 , phase + y * sideLength + 48, 8, 8, colour);
    window -> fillRectangle(phase + x * sideLength + 40 , phase + y * sideLength + 12, 8, 8, colour);
    window -> fillRectangle(phase + x * sideLength + 48 , phase + y * sideLength + 4, 8, 8, colour);
    window -> fillRectangle(phase + x * sideLength + 12 , phase + y * sideLength + 40, 8, 8, colour);
    window -> fillRectangle(phase + x * sideLength + 4  , phase + y * sideLength + 48, 8, 8, colour);
    merge = 22;
    window -> fillRectangle(phase + x * sideLength + merge, phase + y * sideLength + merge , sideLength - merge * 2, sideLength - merge * 2, colour);
    merge = 25;
    window -> fillRectangle(phase + x * sideLength + merge, phase + y * sideLength + merge , sideLength - merge * 2, sideLength - merge * 2, bkColour);
    break;

    case PieceType::Queen:

    window -> fillRectangle(phase + x * sideLength + 4 , phase + y * sideLength + 26, 52, 8, colour);
    window -> fillRectangle(phase + x * sideLength + 26 , phase + y * sideLength + 4, 8, 52, colour);

    window -> fillRectangle(phase + x * sideLength + 4  , phase + y * sideLength + 4, 8, 8, colour);
    window -> fillRectangle(phase + x * sideLength + 12 , phase + y * sideLength + 12, 8, 8, colour);
    window -> fillRectangle(phase + x * sideLength + 40 , phase + y * sideLength + 40, 8, 8, colour);
    window -> fillRectangle(phase + x * sideLength + 48 , phase + y * sideLength + 48, 8, 8, colour);
    window -> fillRectangle(phase + x * sideLength + 40 , phase + y * sideLength + 12, 8, 8, colour);
    window -> fillRectangle(phase + x * sideLength + 48 , phase + y * sideLength + 4, 8, 8, colour);
    window -> fillRectangle(phase + x * sideLength + 12 , phase + y * sideLength + 40, 8, 8, colour);
    window -> fillRectangle(phase + x * sideLength + 4  , phase + y * sideLength + 48, 8, 8, colour);
    merge = 19;
    window -> fillRectangle(phase + x * sideLength + merge, phase + y * sideLength + merge , sideLength - merge * 2, sideLength - merge * 2, bkColour);
    merge = 22;
    window -> fillRectangle(phase + x * sideLength + merge, phase + y * sideLength + merge , sideLength - merge * 2, sideLength - merge * 2, colour);
    merge = 25;
    window -> fillRectangle(phase + x * sideLength + merge, phase + y * sideLength + merge , sideLength - merge * 2, sideLength - merge * 2, bkColour);

  default:
    break;
  }
}

