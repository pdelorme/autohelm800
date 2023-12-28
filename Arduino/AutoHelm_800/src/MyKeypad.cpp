
#define LED 17        // orange
#define COM_KEYS 0    // yellow
#define TEN_KEYS 4    // green
#define ONE_KEYS 16   // blue
#define LEFT_KEYS 2   // red
#define RIGHT_KEYS 5  // brown

enum AHKey {
  KEY_PLUSONE  = '+', 
  KEY_MINUSONE = '-', 
  KEY_PLUSTEN  = '*',
  KEY_MINUSTEN = '/', 
  KEY_AUTO     = 'A', 
  KEY_STANDBY  = 'S', 
  KEY_NONE     = '\0'
};


#include <Arduino.h>

class MyKeypad{
  private:
    AHKey prevKey = KEY_NONE;
    
    AHKey readKey(int gpio, AHKey leftKey, AHKey rightKey){
      digitalWrite(LEFT_KEYS, HIGH);
      digitalWrite(RIGHT_KEYS, LOW);
      int left = digitalRead(gpio);
    
      digitalWrite(LEFT_KEYS, LOW);
      digitalWrite(RIGHT_KEYS, HIGH);
      int right = digitalRead(gpio);
      
      #ifdef KEYPAD_DEBUG
        if(left == HIGH) Serial.println("Key = "+leftKey);
        if(right == HIGH) Serial.println("Key = "+rightKey);
      #endif
      
      if(left == HIGH) return leftKey;
      if(right == HIGH) return rightKey;
      return KEY_NONE;
    }

    void setLed(boolean isOn){
      if(isOn) {
        digitalWrite(LED, LOW);
      } else {
        digitalWrite(LED, HIGH);
      }
    }

    void test(){
      Serial.print("Testing left keys :");
      digitalWrite(LEFT_KEYS, HIGH);
      digitalWrite(RIGHT_KEYS, LOW);
      Serial.print(digitalRead(ONE_KEYS));
      Serial.print(digitalRead(TEN_KEYS));
      Serial.print(digitalRead(COM_KEYS));
      Serial.print("... right keys :");
      digitalWrite(LEFT_KEYS, LOW);
      digitalWrite(RIGHT_KEYS, HIGH);
      Serial.print(digitalRead(ONE_KEYS));
      Serial.print(digitalRead(TEN_KEYS));
      Serial.print(digitalRead(COM_KEYS));
      Serial.println();
      delay(500);
    }

    void testLight(){
      Serial.println("Turning light");
      Serial.println("LED");
      pinMode(LED, OUTPUT);
      digitalWrite(LED, HIGH);
      delay(500);
      digitalWrite(LED, LOW);
      delay(500);

      Serial.println("COM_KEYS");
      pinMode(COM_KEYS, OUTPUT);
      digitalWrite(COM_KEYS, HIGH);
      delay(500);
      digitalWrite(COM_KEYS, LOW);
      delay(500);

      Serial.println("TEN_KEYS");
      pinMode(TEN_KEYS, OUTPUT);
      digitalWrite(TEN_KEYS, HIGH);
      delay(500);
      digitalWrite(TEN_KEYS, LOW);
      delay(500);

      Serial.println("ONE_KEYS");
      pinMode(ONE_KEYS, OUTPUT);
      digitalWrite(ONE_KEYS, HIGH);
      delay(500);
      digitalWrite(ONE_KEYS, LOW);
      delay(500);

      Serial.println("LEFT_KEYS");
      pinMode(LEFT_KEYS, OUTPUT);
      digitalWrite(LEFT_KEYS, HIGH);
      delay(500);
      digitalWrite(LEFT_KEYS, LOW);
      delay(500);

      Serial.println("RIGHT_KEYS");
      pinMode(RIGHT_KEYS, OUTPUT);
      digitalWrite(RIGHT_KEYS, HIGH);
      delay(500);
      digitalWrite(RIGHT_KEYS, LOW);
      delay(500);
    }
  
  public:
    MyKeypad(){
      Serial.print("init My Keypad. DEBUG=");
      #ifdef KEYPAD_DEBUG
          Serial.println("yes");
      #else
          Serial.println("no");
      #endif

      testLight();
      pinMode(LED, OUTPUT);
      pinMode(LEFT_KEYS, OUTPUT);
      pinMode(RIGHT_KEYS, OUTPUT);
      pinMode(COM_KEYS, INPUT);
      pinMode(TEN_KEYS, INPUT);
      pinMode(ONE_KEYS, INPUT);


    }

    /**
     * return the key pressed.
     * if key remain pressed, it will be returned only once.
     */
    AHKey getKey(){
      test();
      AHKey pressedKey = KEY_NONE;
      AHKey oneKey = readKey(ONE_KEYS, KEY_PLUSONE, KEY_MINUSONE);
      if(oneKey != KEY_NONE) {
        pressedKey = oneKey;
        // debug(1, oneKey);
      }/*
      AHKey tenKey = readKey(TEN_KEYS, KEY_PLUSTEN, KEY_MINUSTEN);  
      if(tenKey != KEY_NONE) {
        pressedKey = tenKey;
        debug(2, tenKey);
      }
      AHKey comKey = readKey(COM_KEYS, KEY_AUTO, KEY_STANDBY);
      if(comKey != KEY_NONE) {
        pressedKey = comKey;
        debug(3, comKey);
      }
      setLed(pressedKey !=KEY_NONE);
      */
      AHKey returnedKey = KEY_NONE;
      if(pressedKey!=prevKey){
        returnedKey=pressedKey;
      }
      prevKey=pressedKey;
      // debug(returnedKey);
      return returnedKey;
    }

    void debug(int label, char key){
      #ifdef KEYPAD_DEBUG
        if(key != char(0)) {
          Serial.print("My Keypad");
          Serial.print(label);
          Serial.print(" : ");
          Serial.println((char)key);
        }
      #endif
    }
};

