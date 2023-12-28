//#define MAX471_DEBUG
//#define L298N_DEBUG

// #define HEADING_DEBUG
#define PID_DEBUG
#define KEYPAD_DEBUG
// #define DRV8871_DEBUG
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


AHKeypad* keyboard;
DRV8871*  cylinder;
AHServer* ahServer;
MPU9250*  compass;
Endstops* endstops;

enum Mode {MODE_AUTO, MODE_STANDBY, MODE_GOTO, MODE_NONE};

//Define Variables we'll be connecting to
double pidSetpoint = 0, pidInput = 0, pidOutput;
//Specify the links and initial tuning parameters
double Kp=1, Ki=0.05, Kd=0.25;
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
  myPID.SetOutputLimits(-100, 100);
}

int targetHeading = 0;   // heading direction in degrees
Mode mode = MODE_NONE;

void loop() {
  int shipHeading = compass->getHeading();
  // keyboard
  AHKey key = keyboard->getKey();
  switch (key) {
    case KEY_AUTO : 
      mode = MODE_AUTO;
      targetHeading = shipHeading;
      break;
    case KEY_STANDBY :
      mode = MODE_STANDBY;
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
  // recale target entre 0 et 360°
  if(targetHeading<0)   targetHeading += 360;
  if(targetHeading>360) targetHeading -= 360;
  
  // deviation
  int deltaHeading = targetHeading - shipHeading;

  // recale déviation entre -180 et + 180
  if(deltaHeading>180) deltaHeading = deltaHeading - 360;
  if(deltaHeading<-180) deltaHeading = deltaHeading + 360;

  pidInput = trunc(deltaHeading / 5) * 5;
  myPID.Compute();

  #ifdef HEADING_DEBUG
    if(deltaHeading != 0){
      Serial.print("MODE :");
      Serial.print(mode);
      Serial.print(", Heading :");
      Serial.print(targetHeading);
      Serial.print(", Compass :");
      Serial.print(shipHeading);  
      Serial.print(", Delta :");
      Serial.print(deltaHeading);
      Serial.println();
    }
  #endif
  #ifdef PID_DEBUG
    Serial.print("PID Input : ");
    Serial.print(pidInput);
    Serial.print(", PID Output : ");
    Serial.print(pidOutput);
    Serial.println();
  #endif
  switch(mode){
    case MODE_AUTO :
      cylinder->move(pidOutput);
      break;
    case MODE_STANDBY : 
      break;
    case MODE_NONE:
      break;
  }
  
  // cylinder->loop();
  // current->loop();
  // Serial.println();
  endstops->debug();
  delay(100);
}
