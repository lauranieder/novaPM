/**
 * Serial Basic Reading Example
 * This example reads data from SDS011
 * and sends it to a computer using USB serial communications.
 * 
 * RXD > Arduino digital pin 3
 * TXD > Arduino digital pin 2
 */

#include "novaPM.h"

// Specify your sensor
novaPM sensor();

void setup() {
  // Initialize the sensor
  sensor.begin();

  // Open serial port
  Serial.begin(9600);
}

void loop() {
  // Take a reading
  struct novaPMData data;
  sensor.readData(data);

  // If it was invalid, print error
  if (data.error != 0) {
    Serial.println("Error!");
  } else {
    // Print the values to serial
    Serial.print("PM2.5: ");
    Serial.println(data.pm25);
    Serial.print("PM10: ");
    Serial.println(data.pm10);
    Serial.println("---");
  }

  // Wait a bit so the data is readable
  delay(2500);
}
