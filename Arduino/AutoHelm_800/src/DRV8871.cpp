#include <arduino.h>
#include <Endstops.h>
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
    Endstops* endstops;
    int lastDir = 0; // -1 = left, 1=right

    /**
     * Move id=n direction except if lockeded by endStop
     */
    void doMove(int dir){
      if(dir!=0){
        dir=dir>0?+1:-1;
        if(endstops->getStop() == dir){
          dir=0;
        }
      }
      debug(dir);
      ledcWrite(DRV8871_PWMCHANNEL1, dir<0?MAX_DUTY_CYCLE:0);
      ledcWrite(DRV8871_PWMCHANNEL2, dir>0?MAX_DUTY_CYCLE:0);
    }
    
    int dutyCycle;
    /* Setting PWM Properties */
    int MAX_DUTY_CYCLE = (int)(pow(2, DRV8871_PWMRESOLUTION) - 1);
    int MIN_DUTY_CYCLE = MAX_DUTY_CYCLE*3/4;
  public:
    DRV8871(Endstops* endstops){
      Serial.print("init DRV8871. DEBUG=");
      #ifdef DRV8871_DEBUG
        Serial.println("yes");
        Serial.print("DRV8871 : MIN_DUTY_CYCLE, MAX_DUTY_CYCLE");
        Serial.print(MIN_DUTY_CYCLE);
        Serial.print(", ");
        Serial.println(MAX_DUTY_CYCLE);
      #else
        Serial.println("no");
      #endif
      this->endstops = endstops;
      // IN1
      ledcSetup(DRV8871_PWMCHANNEL1, DRV8871_PWMFREQ, DRV8871_PWMRESOLUTION);
      ledcAttachPin(DRV8871_IN1, DRV8871_PWMCHANNEL1);
      // IN2
      ledcSetup(DRV8871_PWMCHANNEL2, DRV8871_PWMFREQ, DRV8871_PWMRESOLUTION);
      ledcAttachPin(DRV8871_IN2, DRV8871_PWMCHANNEL2);
      // init end stops
   }

   void debug(int dir){
      #ifdef DRV8871_DEBUG
        if(dir!=0){
          Serial.print("DRV8871 : ");
          Serial.println(dir<0?"-":(dir>0?"+":"0"));
        }
      #endif
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
