#ifndef Endstops_h
#define Endstops_h

#include <arduino.h>
class Endstops {
   public:
      virtual void debug();
      virtual boolean isEnd();
      virtual boolean isStart();
      virtual boolean isAny();
      virtual int getStop();
   private:
};
#endif