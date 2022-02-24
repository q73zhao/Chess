#include "game.cc"
#include "grid.h"
#include "supervisor.h"
#include "piece_creator.h"

void Game::attach(Observer<InstructionType>* o) {
  grid -> attach()
}
class Grid;
class Supervisor;
class PieceCreator;
template<typename InstructionType> class Observer;

template<typename InstructionType> class Game {
  Grid* grid;
  Supervisor* supervisor;
  PieceCreator* pieceCreator;

 public:
  virtual ~Game<InstructionType>();
  virtual void attach(Observer<InstructionType>* o);
  virtual bool execute(constInstructionType);
  virtual void init() = 0;
};

#endif
