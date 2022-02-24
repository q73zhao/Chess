#ifndef _OBSERVER_H_
#define _OBSERVER_H_

template<typename InstructionSet> class Grid;
template<typename InstructionSet> class Observer {
  public:
  virtual void notify(const InstructionSet&, const Grid<InstructionSet>&) = 0;
  virtual ~Observer() = default;
};

#endif


