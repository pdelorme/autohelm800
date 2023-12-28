/**
 * Autohelm 800 Keypad
 */
#include <Keypad.h>

#if ARCH == ESP32_WROOM_DEVKIT
  #define LED 17        // orange
  #define COM_KEYS 0    // yellow
  #define TEN_KEYS 4    // green
  #define ONE_KEYS 16   // blue
  #define LEFT_KEYS 2   // red
  #define RIGHT_KEYS 5  // brown
#elif ARCH == ESP32_LOLIN_LITE
  #define LED 13        // orange
  #define ONE_KEYS 12   // yellow
  #define TEN_KEYS 14   // green
  #define COM_KEYS 27   // blue
  #define LEFT_KEYS 25  // red
  #define RIGHT_KEYS 26 // brown
#endif

enum AHKey {
  KEY_PLUSONE  = '+', 
  KEY_MINUSONE = '-', 
  KEY_PLUSTEN  = '*',
  KEY_MINUSTEN = '/', 
  KEY_AUTO     = 'A', 
  KEY_STANDBY  = 'S', 
  KEY_NONE     = '\0'
};

class AHKeypad {
  private:
    static const byte ROWS = 3; //three rows
    static const byte COLS = 2; //two columns
    char keys[ROWS][COLS] = {
      {KEY_MINUSONE, KEY_PLUSONE},
      {KEY_MINUSTEN, KEY_PLUSTEN},
      {KEY_STANDBY , KEY_AUTO}
    };
    byte rowPins[ROWS] = {ONE_KEYS, TEN_KEYS, COM_KEYS}; //connect to the row pinouts of the keypad
    byte colPins[COLS] = {LEFT_KEYS, RIGHT_KEYS}; //connect to the column pinouts of the keypad
    Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
    

  public:
    AHKeypad(){
      Serial.print("init AH Keypad. DeBUG=");
      #ifdef KEYPAD_DEBUG
          Serial.println("yes");
      #else
          Serial.println("no");
      #endif
      pinMode(LED, OUTPUT);
    }

    /**
     * return the key pressed.
     * if key remain pressed, it will be returned only once.
     */
    AHKey getKey(){
      char key = keypad.getKey();
      AHKey ahKey = AHKey(key);
      debug(key);
      return ahKey;
    }

    void light(boolean onoff){
      if(onoff){
        digitalWrite(LED, HIGH);
      } else {
        digitalWrite(LED, LOW);
      }
    }

    void debug(char key){
      #ifdef KEYPAD_DEBUG
        if(key != char(0)) {
          Serial.print("AH Keypad : KEY=");
          Serial.println((char)key);
          light(true);
        } else {
          light(false);
        }
        /*
        if(keypad.isPressed(KEY_PLUSONE))  Serial.print(KEY_PLUSONE);
        if(keypad.isPressed(KEY_MINUSONE)) Serial.print(KEY_MINUSONE);
        if(keypad.isPressed(KEY_PLUSTEN))  Serial.print(KEY_PLUSTEN);
        if(keypad.isPressed(KEY_MINUSTEN)) Serial.print(KEY_MINUSTEN);
        if(keypad.isPressed(KEY_AUTO))     Serial.print(KEY_AUTO);
        if(keypad.isPressed(KEY_STANDBY))  Serial.print(KEY_STANDBY); 
        */
      #endif
    }
};
