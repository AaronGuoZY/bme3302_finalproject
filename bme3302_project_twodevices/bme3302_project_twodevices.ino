/*
  Communication with two devices -- MAX30102 Pulse Ox and ADXL343 accelerometer
*/

#include <Wire.h>
#include "MAX30105.h"
#include <Adafruit_ADXL343.h>
#include <Adafruit_SSD1306.h>

// Construct accelerometer object
Adafruit_ADXL343 accel = Adafruit_ADXL343(420);
// Construct pulse ox object
MAX30105 pulseOx;

// Define global variables
adxl34x_range_t accel_range = ADXL343_RANGE_16_G; // currently set to 16g, change this variable to change range
uint pulse_amp_red = 0x0A;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
//Construct OLED display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Wire.begin(23, 22);  // Initialize the SDA(23) and SCL(22) port
  Serial.begin(115200);  // Set up serial communication with the computer
  
  // Scan for I2C devices, accelerometer has address 0x53 and pulse ox has address 0x57
  Serial.println("\nI2C Scanning\n");
  Serial.println("Look for 0x53 for accelerometer, and 0x53 for pulse oximeter\n");
  i2c_scanner();

  // Initialize the accelerometer
  if(!accel.begin()) {
    Serial.println("$$$$$$$$ No ADXL343 detected. Please check your connections. $$$$$$$$");
  }
  // Set accelerometer range
  accel.setRange(accel_range); 
  Serial.println("---- ADXL343 Accelerometer Information ----");
  accel.printSensorDetails();

  // Initialize the pulse oximeter
  if(!pulseOx.begin()) {
    Serial.println("$$$$$$$$ No MAX30102 detected. Please check your connections. $$$$$$$$");
  }
  pulseOx.setup();
  pulseOx.setPulseAmplitudeRed(pulse_amp_red); //Turn Red LED to low to indicate sensor is running
  pulseOx.setPulseAmplitudeGreen(0); // Turn off green light 

  // Initialize the display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  // Clear display buffer
  display.display();
  display.setTextSize(1.2);
  display.setCursor(0,0);
  display.setTextColor(WHITE);
}

void loop() {
  // Get acceleration measurements
  sensors_event_t event;
  accel.getEvent(&event);

  Serial.println("--------");

  
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

  // Get pulse ox readings
  long irVal = pulseOx.getIR();
  display.setCursor(0,0);
  Serial.print("IR: ");
  Serial.println(irVal);
  Serial.println("--------");

  display.println("--------");
  display.print("IR Value: ");
  display.println(irVal);

  display.print("ax: ");
  display.println(event.acceleration.x);
  display.print("ay: ");
  display.println(event.acceleration.y);
  display.print("az: ");
  display.println(event.acceleration.z);

  display.println("--------");

  display.display();
  display.clearDisplay();
  delay(1000);
}









void i2c_scanner() {
  byte error, address;
  int nDevices;
  Serial.println("-------- I2C Scanner --------");
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
  Serial.println("-----------------------------");
}



void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<256; i++) {
    if(i == '\n') display.write(' ');
    else          display.write(i);
  }

  display.display();
  delay(2000);
}
