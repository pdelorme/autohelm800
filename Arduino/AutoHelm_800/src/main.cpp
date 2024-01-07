//#define MAX471_DEBUG
//#define L298N_DEBUG

#define HEADING_DEBUG
// #define PID_DEBUG
#define KEYPAD_DEBUG
#define DRV8871_DEBUG
#define ENDSTOP_DEBUG

// #define MPU9250_DEBUG
// #define ACS712_DEBUG
// ESP32_WROOM_DEVKIT
// ESP32_LOLIN32_LITE
#define ARCH ESP32_WROOM_DEVKIT

#include <arduino.h>
#include "AHKeypad.cpp"
// #include "MyKeypad.cpp"
#include "AHEndstops.cpp" 
#include "DRV8871.cpp"
#include "AHServer.cpp"
#include "MPU9250.cpp" 
#include <PID_v1.h>

#define MAX_PILOT_VALUE 100
AHKeypad* keyboard;
DRV8871*  cylinder;
AHServer* ahServer;
MPU9250*  compass;
Endstops* endstops;

enum Mode {MODE_AUTO, MODE_STANDBY, MODE_GOTO, MODE_NONE};

//Define Variables we'll be connecting to
double pidSetpoint = 0, pidInput = 0, pidOutput;
//Specify the links and initial tuning parameters
double Kp=4, Ki=3, Kd=2;
PID myPID(&pidInput, &pidOutput, &pidSetpoint, Kp, Ki, Kd, DIRECT);

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("Starting Autohelm Pilot V0");  
  keyboard = new AHKeypad();
  endstops = new AHEndstops();
  cylinder = new DRV8871(endstops);
  ahServer = new AHServer();
  compass  = new MPU9250();
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-MAX_PILOT_VALUE, MAX_PILOT_VALUE);
}

int targetHeading = 0;   // heading direction in degrees
Mode mode = MODE_STANDBY;

/**
 * move motor according to motor limits. 
 */
void move(int delta){
  if(delta>+MAX_PILOT_VALUE) delta = +MAX_PILOT_VALUE;
  if(delta<-MAX_PILOT_VALUE) delta = -MAX_PILOT_VALUE;

  cylinder->move(delta<0?map(delta, -MAX_PILOT_VALUE, 0, -cylinder->getAbsMaxValue(), -cylinder->getAbsMinValue())
                :delta>0?map(delta, 0, MAX_PILOT_VALUE, cylinder->getAbsMinValue(), cylinder->getAbsMaxValue())
                :0);
}

void loop() {
  int shipHeading = compass->getHeading();
  // keyboard
  AHKey key = keyboard->getKey();
  switch (key) {
    case KEY_AUTO :
      mode = MODE_AUTO;
      targetHeading = shipHeading;
      myPID.SetMode(MANUAL);
      myPID.SetMode(AUTOMATIC);
      break;
    case KEY_STANDBY :
      mode = MODE_STANDBY;
      targetHeading = 0;
      break;
    case KEY_PLUSONE :
      targetHeading += 1;
      break;
    case KEY_MINUSONE :
      targetHeading -= 1;
      break;
    case KEY_PLUSTEN : 
      targetHeading +=10;
      break;
    case KEY_MINUSTEN :
      targetHeading -= 10;
      break;
  }

  if(mode==MODE_AUTO) {
    // recale target entre 0 et 360°
    if(targetHeading<0)   targetHeading += 360;
    if(targetHeading>360) targetHeading -= 360;
    
    // deviation
    int deltaHeading = targetHeading - shipHeading;

    // recale déviation entre -180 et + 180
    if(deltaHeading>180) deltaHeading = deltaHeading - 360;
    if(deltaHeading<-180) deltaHeading = deltaHeading + 360;

    #ifdef HEADING_DEBUG
      if(deltaHeading != 0){
        Serial.print("MODE : AUTO , Heading :");
        Serial.print(targetHeading);
        Serial.print(", Compass :");
        Serial.print(shipHeading);  
        Serial.print(", Delta :");
        Serial.print(deltaHeading);
        Serial.println();
      }
    #endif

    pidInput = trunc(deltaHeading / 5) * 5;
    myPID.Compute();
  
    #ifdef PID_DEBUG
      Serial.print("PID Input : ");
      Serial.print(pidInput);
      Serial.print(", PID Output : ");
      Serial.print(pidOutput);
      Serial.println();
    #endif

    move(pidOutput);

  } else if (mode == MODE_STANDBY) {
      move(targetHeading);
      #ifdef HEADING_DEBUG
        if(targetHeading!=0) {
          Serial.print("MODE : STANDBY, Heading :");
          Serial.print(targetHeading);
          Serial.println();
        }
      #endif
  }
  
  // cylinder->loop();
  // current->loop();
  // Serial.println();
  endstops->debug();
  delay(100);
}
