#include "chess_text_monitor.h"
#include "grid.h"
#include "piece.h"
#include <iostream>

static char getSymbol(const ChessISA&);

static char getSymbolOnBlank(int row, int col);
ChessTextMonitor::~ChessTextMonitor() {}
ChessTextMonitor::ChessTextMonitor(int n): gridSize{n} {
  for (int i = 0; i < n; i++) {
    std::vector<char> temp;
    for (int j = 0; j < n; j++) {
      temp.push_back(getSymbolOnBlank(i, j)) ;
    }
    theMonitor.push_back(temp);
  }
}

void ChessTextMonitor::notify(const ChessISA& instruct, const Grid<ChessISA>& grid) {
    //std::cerr << "ChessTextMonitor Notify \n";
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            const Piece<ChessISA>* piece = grid.getPiece(i,j);
            if (piece) {
                theMonitor[i][j] = getSymbol(piece -> getState());
            } else {
                theMonitor[i][j] = getSymbolOnBlank(i,j);
            }
        }
    }
    theGrave.clear();
    for (auto &piece : *(grid.showGrave())) {
        theGrave.push_back(getSymbol(piece -> getState()));
    }
}

static char getSymbol(const ChessISA& state) {
    std::string symbols[] = {"Kk", "Qq", "Bb", "Rr", "Nn", "Pp"};
    return symbols[int(state.pieceType)][int(state.group)];
}

static char getSymbolOnBlank(int row, int col) {
    return ((row + col) % 2)? ' ': '_';
}

void ChessTextMonitor::monitor() {
    std::cout << "   a b c d e f g h\n\n";
    for (int i = 7; i >= 0; i--) {
        std::cout << i + 1 << "  ";
        for (int j = 0; j < 8; j++) {
            std::cout << theMonitor[i][j] << " ";
        }
        std::cout << " " << i + 1 << std::endl;
    }
    std::cout << "\n   a b c d e f g h\n";
    std::cout << "Grave: ";
    for (auto c: theGrave ) {
        std::cout << c << " ";
    }
    std::cout << std::endl; 
}

