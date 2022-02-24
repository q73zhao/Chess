#ifndef _PLAYER_H_
#define _PLAYER_H_
#include <vector>
#include "observer.h"
#include <string>
template<typename InstructionSet> class Player
    : public Observer<InstructionSet> {
    std::string name;
    public:
    Player(std::string name);
    std::string getName();
    virtual InstructionSet InstructionFetch() = 0;
    virtual ~Player();
};

template<typename InstructionSet>
std::string Player<InstructionSet>::getName() {
    return name;
}

template<typename InstructionSet>
Player<InstructionSet>::~Player() {}

template<typename InstructionSet>
Player<InstructionSet>::Player(std::string name) : name{name} {}


#endif

