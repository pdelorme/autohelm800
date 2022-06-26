#include <arduino.h>
/**
 * DRV8871 HBridge Driver.
 */ 
#if ARCH == ESP32_WROOM_DEVKIT
  #define DRV8871_IN1 32    // yellow
  #define DRV8871_IN2 33    // green
#elif ARCH == ESP32_LOLIN_LITE
  #define DRV8871_IN1 16   // yellow
  #define DRV8871_IN2 4    // green
#endif
#define DRV8871_PWMCHANNEL1 0
#define DRV8871_PWMCHANNEL2 1
#define DRV8871_PWMFREQ 10000
#define DRV8871_PWMRESOLUTION 8
#include "ACS712.cpp"

class DRV8871 {
  private:
    // end stop current sensor
    ACS712*   current;
    int lastDir = 0; // -1 = left, 1=right
    boolean endStopped = false;

    /**
     * Move id=n direction except if lockeded by endStop
     */
    void doMove(int dir){
      if(dir!=0){
        dir=dir>0?+1:-1;
        if(endStopped && dir == lastDir){
          return;
        }
        lastDir=dir;
        if(checkEndStop()) {
          dir=0;
        } else {
          endStopped=false;
        }
      }
      #ifdef DRV8871_DEBUG
        Serial.print(dir<0?"-":(dir>0?"+":"0"));
      #endif
      ledcWrite(DRV8871_PWMCHANNEL1, dir<0?MAX_DUTY_CYCLE:0);
      ledcWrite(DRV8871_PWMCHANNEL2, dir>0?MAX_DUTY_CYCLE:0);
    }

    /**
     * stops if end stop reached.
     * returntrue is motor has been stopped.
     */
    boolean checkEndStop(){
      if(current->readAmp()>2.2){
        endStopped=true;
        return true;
      }
      return false;
    }
    
    int dutyCycle;
    /* Setting PWM Properties */
    int MAX_DUTY_CYCLE = (int)(pow(2, DRV8871_PWMRESOLUTION) - 1);
    int MIN_DUTY_CYCLE = MAX_DUTY_CYCLE*3/4;
  public:
    DRV8871(){
      // IN1
      ledcSetup(DRV8871_PWMCHANNEL1, DRV8871_PWMFREQ, DRV8871_PWMRESOLUTION);
      ledcAttachPin(DRV8871_IN1, DRV8871_PWMCHANNEL1);
      // IN2
      ledcSetup(DRV8871_PWMCHANNEL2, DRV8871_PWMFREQ, DRV8871_PWMRESOLUTION);
      ledcAttachPin(DRV8871_IN2, DRV8871_PWMCHANNEL2);
      // init current sensor
      current  = new ACS712();
   }

    void loop(){
      checkEndStop();
    }

    void left(){
      return doMove(-1);
    }

    void right(){
      return doMove(1);
    }

    void move(int dir){
      doMove(dir);
    }
    void stop(){
      return doMove(0);
    }

};
