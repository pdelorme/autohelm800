#include <arduino.h>
/**
 * Cylinder controle using L298N
 */
#define L298N_ENA 17   // blue   
#define L298N_IN1 16   // yellow
#define L298N_IN2 4    // green
#define L298N_PWMCHANNEL 0
#define L298N_PWMFREQ    10000 // 5 KHz
#define L298N_PWMRESOLUTION 8

class L298N {
  private:
    void left(int dutyCycle){
      #ifdef L298N_DEBUG
      Serial.print("-");
      Serial.print(dutyCycle);
      #endif
      digitalWrite(L298N_IN1, HIGH);
      digitalWrite(L298N_IN2, LOW);
      ledcWrite(L298N_PWMCHANNEL, dutyCycle);
    }
    
    void right(int dutyCycle){
      #ifdef L298N_DEBUG
      Serial.print("+");
      Serial.print(dutyCycle);
      #endif
      digitalWrite(L298N_IN1, LOW);
      digitalWrite(L298N_IN2, HIGH);
      ledcWrite(L298N_PWMCHANNEL, dutyCycle);
    }
    
    void stop(){
      digitalWrite(L298N_IN1, LOW);
      digitalWrite(L298N_IN2, LOW);
      ledcWrite(L298N_PWMCHANNEL, 0);
    }

    int dutyCycle;
    /* Setting PWM Properties */
    int MAX_DUTY_CYCLE = (int)(pow(2, L298N_PWMRESOLUTION) - 1);
    int MIN_DUTY_CYCLE = MAX_DUTY_CYCLE*3/4;
  public:
    L298N(){
      pinMode(L298N_IN1, OUTPUT);
      pinMode(L298N_IN2, OUTPUT);  
      ledcSetup(L298N_PWMCHANNEL, L298N_PWMFREQ, L298N_PWMRESOLUTION);
      /* Attach the LED PWM Channel to the GPIO Pin */
      ledcAttachPin(L298N_ENA, L298N_PWMCHANNEL);
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
};
