
AHKeypad* keyboard;
DRV8871*  cylinder;
//ACS712*   current;
INA219*   current2;
// AHServer* ahServer = new AHServer();
enum Mode {MODE_AUTO, MODE_STANDBY, MODE_GOTO, MODE_NONE};

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("Autopilot V0");  
  keyboard = new AHKeypad();
  keyboard->debug();
  cylinder = new DRV8871();
//  current  = new ACS712();
  current2 = new INA219();
}

int delta(int c, int h){
  if(c < 180){
    if(c+180>h){
      return c-h;
    } else {
      return c-h-360;
    }
  } else {
    if(c-180<h){
      return c-h;
    } else {
      return c-h-360;
    }
  }
}

float alpha=0;     // used for simulation.
int heading = 0;   // heading direction in degrees
Mode mode = MODE_NONE;

void loop() {
  int compass = 0; // qmc5883L_loop();
  // keyboard
  AHKey key = keyboard->getKey();
  switch (key) {
    case KEY_AUTO : 
      mode = MODE_AUTO;
      heading = compass;
      break;
    case KEY_STANDBY :
      mode = MODE_STANDBY;
      alpha=0;
      heading = 0;
      break;
    case KEY_PLUSONE :
      heading += 10;
      break;
    case KEY_MINUSONE :
      heading -= 10;
      break;
    case KEY_PLUSTEN : 
      heading = 180;
      break;
    case KEY_MINUSTEN :
      heading = -180;
      break;
  }
  // recale entre 0 et 360°
  if(heading<0)   heading += 360;
  if(heading>360) heading -= 360;
  
  //if(heading>180) heading = 180;
  //if(heading<-180) heading = -180;
  
  if(key != KEY_NONE){
    #ifdef KEYPAD_DEBUG
    Serial.println();
    Serial.print("KEY :");
    Serial.print((char)key);
    Serial.print(",MODE :");
    Serial.print(mode);
    Serial.print(",Heading :");
    Serial.print(heading);
    Serial.print(",Compass :");
    Serial.print(compass);  
    #endif
  }

  switch(mode){
    case MODE_AUTO :
      alpha +=.05;
      compass = sin(alpha)*180;
      cylinder->move(delta(compass,heading));
      break;
    case MODE_STANDBY : 
      compass = 0;
      cylinder->move(delta(compass,heading));
      // heading = 0;
      break;
    case MODE_NONE:
      cylinder->move(delta(0,heading));
  }
  
  // cylinder->loop();
  // current->loop();
  // current2->loop();
  Serial.println();
  delay(100);
}
