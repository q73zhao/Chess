#ifndef _GRID_H_
#define _GRID_H_
#include <iostream>
#include <vector>

#include "piece.h"
template<typename InstructionSet> class Piece;
template<typename InstructionSet> class Observer;

template<typename InstructionSet> class Grid {
  std::vector<std::vector<Piece<InstructionSet>*>> theGrid; 
  std::vector<Piece<InstructionSet>*> theGrave;  // The actual grid.
  std::vector<Observer<InstructionSet>*> observers;
  int size;
 public:
  bool detachObserver(Observer<InstructionSet>* o);
  void notifyObservers(const InstructionSet&);
  Piece<InstructionSet>* getPiece(int row, int col) const;
  void attach(Observer<InstructionSet>* o);
  Piece<InstructionSet>* withdrawal(int row, int col);
  void set(Piece<InstructionSet>*, int row, int col);
  void attachToGrave(Piece<InstructionSet>* p) {theGrave.push_back(p);}
  Piece<InstructionSet>* detachFromGrave();
  const std::vector<Piece<InstructionSet>*>* showGrave() const {return &theGrave;}
  void clear();
  void wash();
  void copy(const Grid& other);
  ~Grid();
  Grid(int size);
};


template <typename InstructionSet> 
void Grid<InstructionSet>::copy(const Grid& other) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      theGrid[i][j] = other.getPiece(i,j);
    }
  }
  theGrave.clear();
}

template <typename InstructionSet> 
void Grid<InstructionSet>::wash() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      theGrid[i][j] = nullptr;
    }
  }
  theGrave.clear();
}



template <typename InstructionSet> 
Piece<InstructionSet>* Grid<InstructionSet>::detachFromGrave() {
    Piece<InstructionSet>* rv = theGrave.back();
    theGrave.pop_back();
    return rv;
}



template <typename InstructionSet> 
void Grid<InstructionSet>::clear() {
      for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            delete theGrid[i][j];
            theGrid[i][j] = nullptr;
        }
    }
    for (auto piece: theGrave) {
      delete piece;
      //std::cout << "delete grave\n";
    }
    theGrave.clear();
}

template <typename InstructionSet> 
bool Grid<InstructionSet>::detachObserver(Observer<InstructionSet>* o) {
  for (unsigned int i = 0; i < observers.size(); i++) {
    if (observers[i] == o) {
      observers.erase(observers.begin() + i);
      //std::cerr << "grid detach observer success\n";
      return true;
    }
  }    
  return false;
}

template <typename InstructionSet> 
void Grid<InstructionSet>::notifyObservers(const InstructionSet& instruct) {
  //std::cerr << "Grid notifyObservers\n";
  //std::cerr << observers.size() << std::endl;
  for (auto &ob : observers) {
    ob -> notify(instruct, *this);
    //std::cout << ob;
  }
}


template<typename InstructionSet> 
Grid<InstructionSet>::Grid(int size) : size{size} {
    for (int i = 0; i < size; i++) {
      std::vector<Piece<InstructionSet>*> temp;
      for (int j = 0; j < size; j++) {
        temp.push_back(nullptr);
      }
    theGrid.push_back(temp);
  }
}

template<typename InstructionSet> 
Piece<InstructionSet>* Grid<InstructionSet>::getPiece(int row, int col) const {
  return theGrid[row][col];
}

template<typename InstructionSet> 
void Grid<InstructionSet>::attach(Observer<InstructionSet>* o) {
  //std::cout << "Grid::attach\n";
  observers.push_back(o);
  //std::cout << observers.size() << std::endl;
}

template<typename InstructionSet> 
Piece<InstructionSet>* Grid<InstructionSet>::withdrawal(int row, int col) {
  Piece<InstructionSet>* rv = theGrid[row][col];
  theGrid[row][col] = nullptr;
  return rv;
}

template<typename InstructionSet> 
void Grid<InstructionSet>::set(Piece<InstructionSet>* p, int row, int col) {
  theGrid[row][col] = p;
}

template<typename InstructionSet> 
Grid<InstructionSet>::~Grid() {
     /* for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        std::cerr << theGrid[i][j];
        delete theGrid[i][j];
      }
  }  }
 
  std:: cerr << "the Grve dtor " << theGrave.size() << std::endl;
  for (auto& piece: theGrave) {
      delete piece;
  }*/
}

#endif
