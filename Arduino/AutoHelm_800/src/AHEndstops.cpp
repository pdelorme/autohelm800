#include <arduino.h>
#include <Endstops.h>
/**
 * Cylinder controle using L298N
 */
#define ENDSTOP_START 13   // DO NOT USE 9
#define ENDSTOP_END 14   // yellow

class AHEndstops : public Endstops {
  private:
  public:
    AHEndstops(){
      Serial.print("init EndStops. DEBUG=");
      #ifdef ENDSTOP_DEBUG
        Serial.println("yes");
      #else
        Serial.println("no");
      #endif
      pinMode(ENDSTOP_START, INPUT_PULLUP);
      pinMode(ENDSTOP_END, INPUT_PULLUP);  
    }

    /**
     * outputs endstops values.
     */
    void debug(){
      #ifdef ENDSTOP_DEBUG
        boolean start = isStart();
        boolean end = isEnd();
        if(start || end){
          Serial.print("Endstop : ");
          if(start){ Serial.print("START "); }
          if(end)  { Serial.print("END"); }
          Serial.println();
        }
      #endif
  }
    
    boolean isEnd(){
      return digitalRead (ENDSTOP_END) == LOW;
    }

    boolean isStart(){
      return digitalRead (ENDSTOP_START) == LOW;
    }

    boolean isAny(){
      return isEnd() || isStart();
    }

    /**
     * returns -1 if start, 1 if end, 0 if none
     */
    int getStop(){
        if(isEnd()) return 1;
        if(isStart()) return -1;
        return 0;
    }
};