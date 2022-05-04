
Keyboard* keyboard = new Keyboard();
Cylinder* cylinder = new Cylinder();
enum Mode {MODE_AUTO, MODE_STANDBY, MODE_GOTO, MODE_NONE};

void setup() {
  Serial.begin(9600);
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
      heading = 0;
      break;
    case KEY_PLUSONE :
      heading += 1;
      break;
    case KEY_MINUSONE :
      heading -= 1;
      break;
    case KEY_PLUSTEN : 
      heading += 10;
      break;
    case KEY_MINUSTEN :
      heading -= 10;
      break;
  }
  // recale entre 0 et 360°
  //if(heading<0)   heading += 360;
  //if(heading>360) heading -= 360;
  
  if(heading>180) heading = 180;
  if(heading<-180) heading = -180;
  
if(key != KEY_NONE){
  Serial.println();
  Serial.print("KEY :");
  Serial.print((char)key);
  Serial.print(",MODE :");
  Serial.print(mode);
  Serial.print(",Heading :");
  Serial.print(heading);
  cylinder->move(heading);
}

  /*
  switch(mode){
    case MODE_AUTO :
      cylinder_move(delta(compass,heading));
      break;
    case MODE_STANDBY : 
      cylinder_move(delta(compass,heading));
      // heading = 0;
      break;
  }
  */
  delay(100);
}
