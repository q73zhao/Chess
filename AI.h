#ifndef _AI_
#define _AI_
#include <vector>
template<typename InstructionSet> class Grid;
template<typename InstructionSet> class AI {
    public:
    virtual ~AI() {};
    virtual InstructionSet suggestion(int level, int currPlayerIndex, const Grid<InstructionSet>&) = 0;
    virtual bool validate(const InstructionSet&, int currPlayerIndex,const Grid<InstructionSet>&) = 0;
    virtual bool evaluate(const Grid<InstructionSet>&, int currPlayerIndex, std::vector<float>&) = 0;
    virtual bool setupCheck(const Grid<InstructionSet>& ) = 0;

};


#endif
