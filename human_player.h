#ifndef _HUMAN_PLAYER_H_
#define _HUMAN_PLAYER_H_
#include "chess_instruction.h"
#include "player.h"
#include <string>
#include <vector>
class HumanPlayer : public Player<ChessISA> {
    Group group;
    std::string command;
    std::vector<std::string> argv;
    std::vector<std::vector<char>> theGrid; 
    ChessISA InstructionDecodeCS246Mode();
    int argc;
    public:
    HumanPlayer();
    HumanPlayer(Group group, std::string name);
    void notify(const ChessISA&, const Grid<ChessISA>&) override;
    ChessISA InstructionFetch() override;

};


#endif

