
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

class DRV8871 {
  private:
    void left(int dutyCycle){
      #ifdef DRV8871_DEBUG
      Serial.print("-");
      Serial.println(dutyCycle);
      #endif
      ledcWrite(DRV8871_PWMCHANNEL1, MAX_DUTY_CYCLE);
      ledcWrite(DRV8871_PWMCHANNEL2, 0);
    }
    
    void right(int dutyCycle){
      #ifdef DRV8871_DEBUG
      Serial.print("+");
      Serial.println(dutyCycle);
      #endif
      ledcWrite(DRV8871_PWMCHANNEL1, 0);
      ledcWrite(DRV8871_PWMCHANNEL2, MAX_DUTY_CYCLE);
    }
    
    int dutyCycle;
    /* Setting PWM Properties */
    static const int MAX_DUTY_CYCLE = (int)(pow(2, DRV8871_PWMRESOLUTION) - 1);
    static const int MIN_DUTY_CYCLE = MAX_DUTY_CYCLE*3/4;
  public:
    DRV8871(){
      // IN1
      ledcSetup(DRV8871_PWMCHANNEL1, DRV8871_PWMFREQ, DRV8871_PWMRESOLUTION);
      ledcAttachPin(DRV8871_IN1, DRV8871_PWMCHANNEL1);
      // IN2
      ledcSetup(DRV8871_PWMCHANNEL2, DRV8871_PWMFREQ, DRV8871_PWMRESOLUTION);
      ledcAttachPin(DRV8871_IN2, DRV8871_PWMCHANNEL2);
   }

    void loop(){
    }
    
    void move(int degree){
      if(degree==0) {
        stop();
        return;
      }
      if(degree<0) left(map(-degree, 0, 180, MIN_DUTY_CYCLE, MAX_DUTY_CYCLE));
      else right(map(degree, 0, 180, MIN_DUTY_CYCLE, MAX_DUTY_CYCLE)); 
    }

    void stop(){
      ledcWrite(DRV8871_PWMCHANNEL1, 0);
      ledcWrite(DRV8871_PWMCHANNEL2, 0);
    }

};
