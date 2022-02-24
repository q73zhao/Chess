#include <iostream>
#include <string>
#include "grid.h"
#include "chess.h"
#include "player.h"
#include "human_player.h"
#include "game.h"
#include "chess_instruction.h"
#include "chess_piece_creator.h"
#include "game_controller.h"
#include "chess_AI.h"
#include "chess_text_monitor.h"
#include "chess_graphics_monitor.h"
using namespace std;


int main(int argc, char *argv[]) {

  std::vector<float> score = {0,0};
  string cmd;

  Chess* chess = new Chess;
  GameController<ChessISA>* broker = new GameController<ChessISA>{chess};
  ChessTextMonitor* td = new ChessTextMonitor{8};
  ChessGraphicsMonitor* gd = new ChessGraphicsMonitor;
  chess -> attachMonitor(td);
  chess -> attachMonitor(gd);



  cout << "Enter \"setup\" to enter Setup Mode\n";
  cout << "Press Ctrl + D to exit\n";
  cout << "Enter \"game white-player black-player\" to start a new game.\n";
  cout << "player can be either \"human\" or \"computer[0-4]\".\n";

  while (cin >> cmd) {        
  if (cmd == "game") {
    int numPlayer = 0;
    string player;
    while (cin >> player) {
      if (player == "human") {
          broker -> addPlayer(new HumanPlayer);
          numPlayer++;
          std::cout << "Add player " << numPlayer << ": " << player<< " successful\n";
      } else if (player.substr(0,9) == "computer[" && player[10] == ']') {
          broker -> addAIPlayer(player[9] - '0');
          numPlayer++;
          std::cout << "Add player " << numPlayer << ": " << player<< " successful\n";
      } else {
        std::cout << "Add player " << numPlayer << " Faild\n";
      }
      if (numPlayer == 2) { break; }
    }

    broker -> gameStart();
    score[0] += broker -> scoreBoard()[0];
    score[1] += broker -> scoreBoard()[1];
    broker -> clearPlayer();
  } else if (cmd == "setup") {
    broker -> setup();
  } else {
    continue;
  }

  cout << "Enter \"setup\" to enter Setup Mode\n";
  cout << "Press Ctrl + D to exit\n";
  cout << "Enter \"game white-player black-player\" to start a new game.\n";
  cout << "player can be either \"human\" or \"computer[0-4]\".\n";

  }
  
  cout << "Final Score:\nWhite: " << score[0] << endl << "Black: " << score[1] << endl;
  delete broker;
}

