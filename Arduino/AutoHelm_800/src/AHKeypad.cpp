/**
 * Autohelm 800 Keypad
 */
#include <Keypad.h>

#if ARCH == ESP32_WROOM_DEVKIT
  #define LED 2         // orange
  #define COM_KEYS 16   // yellow
  #define TEN_KEYS 4    // green
  #define ONE_KEYS 0    // blue
  #define LEFT_KEYS 5   // red
  #define RIGHT_KEYS 17 // brown
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
    static const byte rows = 3; //three rows
    static const byte cols = 2; //two columns
    char keys[rows][cols] = {
      {KEY_MINUSONE, KEY_PLUSONE},
      {KEY_MINUSTEN, KEY_PLUSTEN},
      {KEY_STANDBY , KEY_AUTO}
    };
    byte rowPins[rows] = {ONE_KEYS, TEN_KEYS, COM_KEYS}; //connect to the row pinouts of the keypad
    byte colPins[cols] = {LEFT_KEYS, RIGHT_KEYS}; //connect to the column pinouts of the keypad
    Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );
  public:
    AHKeypad(){}

    /**
     * return the key pressed.
     * if key remain pressed, it will be returned only once.
     */
    AHKey getKey(){
      char key = keypad.getKey();
      AHKey ahKey = AHKey(key);
      return ahKey;
    }

    void debug(){
      Serial.println("AH Keypad");
    }
};
