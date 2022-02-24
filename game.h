#ifndef _GAME_H_
#define _GAME_H_
#include <iostream>
#include <vector>
#include <utility>
#include <memory>

#include "monitor.h"
template<typename InstructionSet> class Grid ;
template<typename InstructionSet> class Monitor;
template<typename InstructionSet> class PieceCreator;
template<typename InstructionSet> class Player;
template<typename InstructionSet> class AI;

template<typename InstructionSet> class Game {
  protected:
  Grid<InstructionSet>* grid;
  PieceCreator<InstructionSet>* pieceCreator;
  AI<InstructionSet>* ai;
  std::vector<std::pair<InstructionSet, bool>> instructionHistory;
  std::vector<Monitor<InstructionSet>*> monitor;
  std::vector<InstructionSet> setupInstruction;
  std::vector<int> playersLevel;
  std::vector<float> playerScore;
  int currPlayer = 0;
  int numPlayer = 0;
  const int maxPlayer;

  public:
  Game(Grid<InstructionSet>* , PieceCreator<InstructionSet>* pc, AI<InstructionSet>* ai, int maxPlayer);
  virtual ~Game();

  void flushMonitor();
  void attachMonitor(Monitor<InstructionSet>* o);
  bool attachPlayer(Player<InstructionSet>*);
  bool attachAIPlayer(int level);
  bool detachObserver(Observer<InstructionSet>*);
  void clearPlayer();
  int currPlayerIndex();
  std::vector<float> getScore();
  virtual void setup() = 0;
  virtual bool execute(const InstructionSet&, bool validationCheck = true) = 0;
  virtual bool isFinish() = 0;
  virtual void init() = 0;
  virtual void clear() = 0;
};

template <typename InstructionSet> 
bool Game<InstructionSet>::detachObserver(Observer<InstructionSet>* o) {
  return grid -> detachObserver(o);
}

template <typename InstructionSet> 
void Game<InstructionSet>::clearPlayer() {
  currPlayer = 0;
  numPlayer = 0;
  playerScore.clear();
  playersLevel.clear();
}

template <typename InstructionSet> 
std::vector<float> Game<InstructionSet>::getScore() {
  return playerScore;
}


template <typename InstructionSet> 
void Game<InstructionSet>::flushMonitor() {
  //std::cout << observers.size() << std::endl;
  for (auto &ob : monitor) {
    ob -> monitor();
    //std::cout << ob;
  }
}

template<typename InstructionSet> 
Game<InstructionSet>::Game(Grid<InstructionSet>* g,
   PieceCreator<InstructionSet>* pc, AI<InstructionSet>* ai, int maxPlayer)
  : grid{g}, pieceCreator{pc}, ai{ai}, maxPlayer{maxPlayer} {}

template<typename InstructionSet> 
Game<InstructionSet>::~Game() {
  delete grid;
  delete pieceCreator;
  delete ai;
  for (auto dp: monitor) {
      delete dp;
  }
}

template<typename InstructionSet> 
int Game<InstructionSet>::currPlayerIndex() {
  return currPlayer;
}

template<typename InstructionSet> 
void Game<InstructionSet>::attachMonitor(Monitor<InstructionSet>* o) {
  grid -> attach(o);
  monitor.push_back(o);
}


template<typename InstructionSet> 
bool Game<InstructionSet>::attachPlayer(Player<InstructionSet>* o) {
  if (numPlayer == maxPlayer) {
        return false;
  }
  grid -> attach(o);
  playersLevel.push_back(-1);
  playerScore.push_back(0);
  numPlayer++;

  return true;
}


template<typename InstructionSet> 
bool Game<InstructionSet>::attachAIPlayer(int level) {
  if (numPlayer == maxPlayer) {
        return false;
  }
  playersLevel.push_back(level);
  playerScore.push_back(0);
  numPlayer++;
  return true;
}
#endif

