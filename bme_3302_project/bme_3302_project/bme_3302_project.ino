#include <Wire.h>
//#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL343.h>


/* Assign a unique ID to this sensor at the same time */
/* Uncomment following line for default Wire bus      */
Adafruit_ADXL343 accel = Adafruit_ADXL343(12345);


void setup(void) {
  Wire.begin(23, 22); // SDA = 23, SCL = 22
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Accelerometer Test");
  Serial.println("");

  // Initialization
  if(!accel.begin()) {
    Serial.println("No ADXL345 detected");
  }

  accel.setRange(ADXL343_RANGE_16_G);

  accel.printSensorDetails();
  Serial.println("");
}


void loop(void) {
  sensors_event_t event;
  accel.getEvent(&event);

  Serial.print("X: ");
  Serial.print(event.acceleration.x);
  Serial.print("|");
  Serial.print("Y: ");
  Serial.print(event.acceleration.y);
  Serial.print("|");
  Serial.print("Z: ");
  Serial.print(event.acceleration.z);
//  Serial.print("|");
  Serial.println("");
  Serial.println("");
  delay(200);
}
