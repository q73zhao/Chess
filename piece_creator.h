#ifndef _PIECE_CREATOR_H_
#define _PIECE_CREATOR_H_
#include <iostream>
#include <vector>
template<class InstructionSet> class Piece;

template<typename InstructionSet> class PieceCreator {
 public:
    virtual ~PieceCreator() {};
    virtual Piece<InstructionSet>* generate(const InstructionSet&) = 0;
};

#endif
