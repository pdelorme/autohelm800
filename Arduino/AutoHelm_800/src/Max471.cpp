#include <arduino.h>
/**
 * MAX471 Current Sensor
 */

#if ARCH == ESP32_WROOM_DEVKIT
  #define MAX471_VOLT_PIN 13
  #define MAX471_AMP_PIN  12
#elif ARCH == ESP32_LOLIN_LITE
  #define MAX471_VOLT_PIN 34
  #define MAX471_AMP_PIN  35
#endif
#define MAX471_AMP_CALIBRATION  1
#define MAX471_VOLT_CALIBRATION 1

class Max471 {
  private:
  public:
    Max471(){
      pinMode (MAX471_VOLT_PIN, INPUT);
      pinMode (MAX471_AMP_PIN, INPUT);
    }

    float readAmp(){
      int A= analogRead (MAX471_AMP_PIN)/MAX471_AMP_CALIBRATION; 
      #ifdef MAX471_DEBUG
      Serial.print ("\t");
      Serial.print (A);
      Serial.print (" A");
      #endif
      return (float)A;
    }

    float readVolt(){
      int V= analogRead (MAX471_VOLT_PIN)/MAX471_VOLT_CALIBRATION; 
      #ifdef MAX471_DEBUG
      Serial.print ("\t");
      Serial.print (V);
      Serial.print (" V");
      #endif
      return (float)V;
    }

    void loop(){
      readAmp();
      readVolt();
      #ifdef MAX471_DEBUG
      Serial.println();
      #endif
    }
};
