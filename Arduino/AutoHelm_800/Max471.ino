#define VOLT_PIN 34
#define AMP_PIN  35
#define AMP_CALIBRATION  1
#define VOLT_CALIBRATION 1

class Max471 {
  private:
  public:
    Max471 () {
      pinMode (VOLT_PIN, INPUT);
      pinMode (AMP_PIN, INPUT);
    }

    float readAmp(){
      int A= analogRead (AMP_PIN)/AMP_CALIBRATION; 
      #ifdef MAX471_DEBUG
      Serial.print ("\t");
      Serial.print (A);
      Serial.print (" A");
      #endif
      return (float)A;
    }

    float readVolt(){
      int V= analogRead (VOLT_PIN)/VOLT_CALIBRATION; 
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
