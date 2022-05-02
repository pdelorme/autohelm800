/**
 * Cylinder controle using L298N
 */
#define borneENA 17   // blue   
#define borneIN1 16  // yellow
#define borneIN2 4    // green

void cylinder_setup(){
  // pinMode(borneENA, OUTPUT);
  pinMode(borneIN1, OUTPUT);
  pinMode(borneIN2, OUTPUT);  
}

void left(int speed){
  digitalWrite(borneIN1, HIGH);
  digitalWrite(borneIN2, LOW);
  analogWrite(borneENA, speed);
  Serial.print("LEFT");
}

void right(int speed){
  digitalWrite(borneIN1, LOW);
  digitalWrite(borneIN2, HIGH);
  analogWrite(borneENA, speed);
  Serial.print("RIGHT");

}

void stop(){
  digitalWrite(borneIN1, LOW);
  digitalWrite(borneIN2, LOW);
  analogWrite(borneENA, 0);
}

void cylinder_move(int degree){
  if(degree==0) {
    stop();
    return;
  }
  Serial.print("Turning :");
  Serial.print(degree);
  if(degree<0) left(map(-degree, 0, 180, 60, 255));
  else right(map(degree, 0, 180, 60, 255));
  Serial.print(" / ");  
}
