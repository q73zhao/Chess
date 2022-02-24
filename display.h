#ifndef _DISPLAY_H_
#define _DISPLAY_H_
#include <iostream>
#include <vector>
#include "observer.h"

template <class InstructionSet>
class Display: public Observer<InstructionSet> {
  public:
  virtual void display() = 0;
};
#endif

