#ifndef _Monitor_H_
#define _Monitor_H_
#include <iostream>
#include <vector>
#include "observer.h"

template <class InstructionSet>
class Monitor: public Observer<InstructionSet> {
  public:
  virtual void monitor() = 0;
};
#endif

