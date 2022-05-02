#include <Wire.h>
#include <QMC5883LCompass.h>

QMC5883LCompass qmc5883L;

void qmc5883L_setup(){
  Wire.begin(23,19); // sda, scl // default ? 23, 19 // 16,17
  qmc5883L.init();
}

/**
 * return current compass value.
 */
int qmc5883L_loop(){
  qmc5883L.read();
  int compass = qmc5883L.getAzimuth();
  Serial.print(" compass :");
  Serial.print(compass);
  Serial.print(" / ");
  return compass;
}
