#ifndef _GAME_GAMECONTROLLER_H_
#define _GAME_GAMECONTROLLER_H_
#include <iostream>
#include <vector>
#include "player.h"
#include "game.h"
#include "observer.h"
template<typename InstructionSet> class Game; 
template<typename InstructionSet> class Player;


template<typename InstructionSet> class GameController {
  Game<InstructionSet>* game;
  std::vector<Player<InstructionSet>*> players;

 public:
  bool addPlayer(Player<InstructionSet>*);
  void clearPlayer();
  bool addAIPlayer(int level);
  void gameStart(void);
  void setup();
  std::vector<float> scoreBoard();
  GameController(Game<InstructionSet>*);
  ~GameController();

};
template<class InstructionSet> 
void GameController<InstructionSet>::clearPlayer() {
  game -> clearPlayer();
  for (auto player: players) {
    game -> detachObserver(player);
    delete player;
  }
  players.clear();
}

template<class InstructionSet> 
std::vector<float> GameController<InstructionSet>::scoreBoard() {
  return game -> getScore();
}


template<class InstructionSet> 
GameController<InstructionSet>::~GameController() {
  delete game;
  //std::cerr << "delete game successful\n"; 
  for (auto player: players) {
    delete player;
  }
}

template<class InstructionSet> 
GameController<InstructionSet>::GameController(Game<InstructionSet>* game)
  : game{game} {}


template<class InstructionSet> 
bool GameController<InstructionSet>::addPlayer(Player<InstructionSet>* player) {
  if (game -> attachPlayer(player)) {
    players.push_back(player);
   
    return true;
  }
  //std::cerr << "GameController::attachPlayer unsuccessful\n";
  return false;
}

template<class InstructionSet> 
bool GameController<InstructionSet>::addAIPlayer(int level) {
  if (game -> attachAIPlayer(level)) {
    players.push_back(nullptr);

    return true;
  }
  //std::cerr << "GameController::attachAIPlayer unsuccessful\n";
  return false;
}

template<class InstructionSet> 
void  GameController<InstructionSet>::gameStart() {
  game -> init();
  //std::cerr << "GameController::gameStart init() done\n";
  int currPlayerIndex;

  while(!game -> isFinish()) {
    currPlayerIndex = game -> currPlayerIndex();
    std::cout << "Current Player Index: " << currPlayerIndex << std::endl;
    InstructionSet instruct = players[currPlayerIndex] -> InstructionFetch();

    game -> execute(instruct);
    //std::cerr << "Instruction Executed Successfully\n";
    game -> flushMonitor();
  }
  //std::cout << "GameController::gameStart finishing \n";
  game -> clear();
}


template<class InstructionSet> 
void  GameController<InstructionSet>::setup() {
  game -> setup();
}
/* 
template<class InstructionSet> 
void GameBroker<InstructionSet>::setMonitor(Observer<InstructionSet>* dp) {
  game -> attach(dp);
  textMonitors.emplace_back(dp);
}*/

#endif

