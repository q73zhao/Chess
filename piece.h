#ifndef _PIECE_H_
#define _PIECE_H_
#include <iostream>
#include <string>
#include <vector>
#include <utility>
template<class InstructionSet> class Grid;
template<class InstructionSet> class Piece {
    int numMoved = 0;
    public:
    virtual ~Piece() {};
    void increaseNumMoved() {numMoved++;}
    void decreaseNumMoved() {numMoved--;}
    virtual InstructionSet getState() const = 0;
    bool isMoved() const {return numMoved;}
    virtual std::vector<std::pair<int,int>> getRange(InstructionSet, const Grid<InstructionSet>&) const = 0;
};

#endif
