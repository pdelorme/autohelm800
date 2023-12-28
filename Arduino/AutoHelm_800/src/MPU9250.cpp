#include <MPU9250_WE.h>
#include <Wire.h>
#define MPU9250_ADDR 0x68

class MPU9250 {
    private:
        MPU9250_WE myMPU9250 = MPU9250_WE(MPU9250_ADDR);

    public:
        MPU9250() {
            Serial.print("init MPU9250. DEBUg=");
            #ifdef MPU9250_DEBUG
                Serial.println("yes");
            #else
                Serial.println("no");
            #endif
            Wire.begin();
            if(!myMPU9250.init()){
                Serial.println(" MPU9250 does not respond");
                byte whoAmICode = myMPU9250.whoAmI();
                Serial.print("WhoAmI Register: 0x");
                Serial.println(whoAmICode, HEX);
            }
            else{
                Serial.println(" MPU9250 is connected");
            }
  
            myMPU9250.setAccOffsets(-14240.0, 18220.0, -17280.0, 15590.0, -20930.0, 12080.0);
            if(!myMPU9250.initMagnetometer()){
                Serial.println(" Magnetometer does not respond");
            }
            else{
                Serial.println(" Magnetometer is connected");
            }
            /* You can choose the following operational modes
            * AK8963_PWR_DOWN            power down (default)
            * AK8963_CONT_MODE_8HZ       continuous at 8Hz sample rate
            * AK8963_CONT_MODE_100HZ     continuous at 100Hz sample rate 
            * 
            * In trigger mode the AK8963 goes into power down after the measurement
            */
            myMPU9250.setMagOpMode(AK8963_CONT_MODE_100HZ);
            
            /* In continuous mode you need to wait for the first data to be available. If you 
            * comment the line below you will probably obtain zero. 
            */
            delay(200);
        }
        /**
         * return heading in degree
         */
        int getHeading() {
            xyzFloat mag = myMPU9250.getMagValues(); // returns magnetic flux density [µT] 
            float pitch = myMPU9250.getPitch() * DEG_TO_RAD;
            float roll  = myMPU9250.getRoll() * DEG_TO_RAD;
            float heading = atan2(mag.y, mag.x) * RAD_TO_DEG;
            float xh = mag.x*cos(pitch) + mag.y*sin(roll)*sin(pitch) - mag.z*cos(roll)*sin(pitch);
            float yh = mag.y*cos(roll) + mag.z*sin(roll);
            float filtered_heading = atan2(yh, xh) * RAD_TO_DEG;
            debug(mag, pitch, roll, heading, filtered_heading);
            return heading;
        }

        void debug(xyzFloat magValue, float pitch, float roll, float heading, float filtered_heading){
            #ifdef MPU9250_DEBUG
                Serial.print("MPU9250 ");
                /*
                Serial.print("Mag : ");
                Serial.print(magValue.x);
                Serial.print(", ");
                Serial.print(magValue.y);
                Serial.print(", ");
                Serial.print(magValue.z);
                Serial.print(" ; ");
                */
                Serial.print("Pitch : ");
                Serial.print(pitch);

                Serial.print("; Roll : ");
                Serial.print(roll);

                Serial.print("; heading : ");
                Serial.print(heading);

                Serial.print("; Filtered : ");
                Serial.println(filtered_heading);
            #endif
        }
};