#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219(0x44);

class INA219 {
  private:

  public:
    INA219(){
      uint32_t currentFrequency;
      ina219.begin();
      Serial.println("Measuring voltage and current with INA219");
    }

    void scan() {
      Serial.println();
      Serial.println("Start I2C scanner ...");
      Serial.print("\r\n");
      byte count = 0;
      
      Wire.begin();
      for (byte i = 8; i < 120; i++)
      {
        Wire.beginTransmission(i);
        if (Wire.endTransmission() == 0)
          {
          Serial.print("Found I2C Device: ");
          Serial.print(" (0x");
          Serial.print(i, HEX);
          Serial.println(")");
          count++;
          delay(1);
          }
      }
      Serial.print("\r\n");
      Serial.println("Finish I2C scanner");
      Serial.print("Found ");
      Serial.print(count, HEX);
      Serial.println(" Device(s).");
    }
    
    void getData(){
      long time_s=millis()/(1000); // convert time to sec
      float busVoltage_V = ina219.getBusVoltage_V();
      float shuntVoltage_mV = ina219.getShuntVoltage_mV();
      float voltage_V = busVoltage_V + (shuntVoltage_mV / 1000);
      float current_mA = ina219.getCurrent_mA();
      //power_mW = ina219.getPower_mW(); 
      float power_mW=current_mA*voltage_V; 
      float energy_Wh=(power_mW*time_s)/3600;   //energy in watt hour
       
      
      //Serial.print("\tBus: "); Serial.print(busVoltage_V); Serial.print(" V");
      //Serial.print("\tShunt: "); Serial.print(shuntVoltage_mV); Serial.print(" mV");
      //Serial.print("\tLoad: "); Serial.print(voltage_V); Serial.print(" V");
      Serial.print("\t"); Serial.print(current_mA/1000); Serial.print(" A_I2C");
      //Serial.print("\tPower: "); Serial.print(power_mW*1000); Serial.print(" W");  
      //Serial.println("----------------------------------------------------------------------------");
    }

    void loop(){
      getData();
    }
    
};
