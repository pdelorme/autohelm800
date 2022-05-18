#ifdef MY_KEYBOARD

#define LED 13       // orange
#define ONE_KEYS 12  // yellow
#define TEN_KEYS 14  // green
#define COM_KEYS 27  // blue

enum AHKey {
  KEY_PLUSONE  = '+', 
  KEY_MINUSONE = '-', 
  KEY_PLUSTEN  = '*',
  KEY_MINUSTEN = '/', 
  KEY_AUTO     = 'A', 
  KEY_STANDBY  = 'S', 
  KEY_NONE     = '\0'
};

class Keyboard{
  private:
    AHKey prevKey = KEY_NONE;
    
    AHKey readKey(int gpio, AHKey lowKey, AHKey highKey){
      int value = analogRead(gpio);
      /*if(value!=0) {
        Serial.print(":");
        Serial.print(value);
      }
      */
      if(value< 100) return KEY_NONE;
      if(value<1500) return lowKey;
      return highKey;
    }

    void setLed(boolean isOn){
      if(isOn) {
        digitalWrite(LED, LOW);
      } else {
        digitalWrite(LED, HIGH);
      }
    }
  
  public:
    Keyboard(){
      pinMode(LED, OUTPUT);
    }

    /**
     * return the key pressed.
     * if key remain pressed, it will be returned only once.
     */
    AHKey getKey(){
      AHKey oneKey = readKey(ONE_KEYS, KEY_PLUSONE, KEY_MINUSONE);
      AHKey tenKey = readKey(TEN_KEYS, KEY_PLUSTEN, KEY_MINUSTEN);  
      AHKey comKey = readKey(COM_KEYS, KEY_AUTO, KEY_STANDBY);
      AHKey pressedKey = KEY_NONE;
      if(oneKey != KEY_NONE) pressedKey = oneKey;
      if(tenKey != KEY_NONE) pressedKey = tenKey;
      if(comKey != KEY_NONE) pressedKey = comKey;
      setLed(pressedKey !=KEY_NONE);
      AHKey returnedKey = KEY_NONE;
      if(pressedKey!=prevKey){
        returnedKey=pressedKey;
      }
      prevKey=pressedKey;
      return returnedKey;
    }

    void debug(){
      Serial.println("My Keyboard");
    }
};
#endif
