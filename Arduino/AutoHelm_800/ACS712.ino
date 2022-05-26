/**
 * ACS712 Current Sensor
 */

#if ARCH == ESP32_WROOM_DEVKIT
  #define ACS712_AMP_PIN 35 // yellow
#elif ARCH == ESP32_LOLIN_LITE
  #define ACS712_AMP_PIN 34 // yellow
#endif
#define ACS712_VOLTS_OFFSET 2.5
#define ACS712_AMPperVOLTS  0.185

class ACS712 {
  private:
    static const int nbValues=5;
    int zeroCalibration=0;
    float values[nbValues];
    int lastIndex=1;
    int firstIndex=0;
    float sumValue=0;

    /**
     * returns average based on the last pushed values.
     */
    float push(float value){
      sumValue+=value;
      sumValue-=values[lastIndex++];
      if(lastIndex>=nbValues)
        lastIndex=0;
      values[firstIndex++]=value;
      if(firstIndex>=nbValues)
        firstIndex=0;
      return sumValue/(nbValues-1);
    }
    float maximum(){
      float maximum=-10;
      for(int i=0;i<nbValues;i++){
        maximum=max(maximum,values[i]);
      }
      return maximum;
    }
    void calibrate(){
      int acc = 0;
      for(int i = 0;i<10;i++){
        acc+=analogRead (ACS712_AMP_PIN);
        delay(10);
      }
      zeroCalibration=acc/10;
      Serial.print ("zero =");
      Serial.println (zeroCalibration);
    }
    
  public:
    ACS712 () {
      // pinMode (ACS712_AMP_PIN, INPUT);
      adcAttachPin(ACS712_AMP_PIN);
      for(int i = 0;i<nbValues;i++){
        values[i]=0;
      }
      calibrate();
    }
    float sup = 0;
    float readAmp(){
      //analogRead (ACS712_AMP_PIN);
      int raw = analogRead (ACS712_AMP_PIN);
      float V = ((float)raw)/4096*3.300;
      float A = (V-2.4)/ACS712_AMPperVOLTS; 
      float avg = push(A);
      float maxi = maximum();
      sup = max(maxi,sup);
      #ifdef ACS712_DEBUG
        //Serial.print ("\t");
        //Serial.print (raw);
        //Serial.print (" C");
        //Serial.print ("\t"); Serial.print (V); Serial.print (" V");
        Serial.print ("\t A_PIN :"); Serial.print (A);
        Serial.print ("\t AVG :"); Serial.print (avg);
        Serial.print ("\t MAX :"); Serial.print (maxi);
        Serial.print ("\t SUP :"); Serial.print (sup);
        Serial.print ("\t TRIG :"); Serial.print (2);
      #endif
      return avg;
    }

    void loop(){
      readAmp();
      #ifdef ACS712_DEBUG
      Serial.println();
      #endif
    }
};
