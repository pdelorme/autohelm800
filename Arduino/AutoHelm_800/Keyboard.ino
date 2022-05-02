#define LED 13       // orange
#define ONE_KEYS 12  // yellow
#define TEN_KEYS 14  // green
#define COM_KEYS 27  // blue

void keyboard_setup(){
    pinMode(LED, OUTPUT);
}

void setLed(boolean isOn){
  if(isOn) {
    digitalWrite(LED, LOW);
  } else {
    digitalWrite(LED, HIGH);
  }
}

Key readKey(int gpio, Key lowKey, Key highKey){
  int value = analogRead(gpio);
  Serial.print(":");
  Serial.print(value);
  Serial.print(":");
  if(value< 100) return KEY_NONE;
  if(value<1500) return lowKey;
  return highKey;
}

Key getKey(){
  Key oneKey = readKey(ONE_KEYS, KEY_PLUSONE, KEY_MINUSONE);
  Key tenKey = readKey(TEN_KEYS, KEY_PLUSTEN, KEY_MINUSTEN);  
  Key comKey = readKey(COM_KEYS, KEY_AUTO, KEY_STANDBY);
  setLed(oneKey!=KEY_NONE || tenKey!=KEY_NONE || comKey!=KEY_NONE);
/*
  Serial.print("ONE :");
  Serial.print(oneKey);
  Serial.print(", TEN :");
  Serial.print(tenKey);
  Serial.print(", COM :");
  Serial.print(comKey);
  Serial.print(" / ");
*/
  if(oneKey != KEY_NONE) return oneKey;
  if(tenKey != KEY_NONE) return tenKey;
  return comKey;
}
