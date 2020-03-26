/*
* novaPM Arduino Library
* Based on Hackair Arduino Library https://www.hackair.eu/docs/hackair-home-v1/arduino-library/
* Modified by Fragmentin --> fragment.in
*/

#include "Arduino.h"
#include "novaPM.h"

SoftwareSerial _serial(PIN_SERIAL_RX, PIN_SERIAL_TX);

/* Constructors */
novaPM::novaPM() {
}

void novaPM::begin() {
  _serial.begin(9600);
}

void novaPM::readData(novaPMData &data) {

        // SDS011
        char index = 0;
        while (_serial.available() && index != 10) {
            _buff[index] = _serial.read();
            index++;
        }
        while (_serial.read() != -1) {}

        // Check package integrity
        if (_buff[0] == 0xAA && _buff[1] == 0xC0 && _buff[9] == 0xAB) {
            int receiveSum = 0;

            for (int i = 2; i < 8; i++) {
                receiveSum += _buff[i];
            }

            if ((receiveSum & 0xFF) == _buff[8]) {
                // Set the error flag
                data.error = H_NO_ERROR;

                // Set data
                data.pm25 = ((_buff[3] << 8) + _buff[2]) / 10.0f;
                data.pm10 = ((_buff[5] << 8) + _buff[4]) / 10.0f;

                return;
            } else {
                // Invalid package, set the error flag
                data.error = H_ERROR_SENSOR;
                return;
            }
        }


    // Invalid sensor ID means something surely went wrong
    data.error = 1;
}

void novaPM::readAverageData(novaPMData &data, uint8_t n) {
    float sum_pm25 = 0.0f;
    float sum_pm10 = 0.0f;
    uint8_t successes = 0;

    for (uint8_t i = 0; i < n; i++) {
        delay(1000); // Wait for a new reading, all sensors have a maximum
                     // 1Hz sampling rate
        readData(data);

        if (data.error == H_NO_ERROR) {
            sum_pm25 += data.pm25;
            sum_pm10 += data.pm10;

            successes += 1;
        }
    }

    data.pm25 = sum_pm25 / successes;
    data.pm10 = sum_pm10 / successes;

    if (successes != n) {
        data.error = H_ERROR_SENSOR_ONCE;
    } else if (successes == 0) {
        data.error = H_ERROR_SENSOR;
    } else {
        data.error = H_NO_ERROR;
    }
}

void novaPM::clearData(novaPMData &data) {
    data.pm25 = 0;
    data.pm10 = 0;
    data.error = H_NO_ERROR;
    data.tamper = 0;
    data.battery = 0;
}

void novaPM::turnOn() {
  // SDS011 uses the built-in power saving function
  // Send anything to wake up the sensor
  _serial.write(0x01);
  delay(2000);
}

void novaPM::turnOff() {
  // SDS011 uses the built-in power saving function
  // Send sleep command
  uint8_t sleep_command[] = {0xAA, 0xB4, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x05, 0xAB};
  for (uint8_t i = 0; i < 19; i++) {
      _serial.write(sleep_command[i]);
  }

  // Discard response
  _serial.flush();
  while (_serial.read() != -1) {}
}

// Normalization function written by Zbyszek Kiliański, Piotr Paul
// https://github.com/piotrkpaul/esp8266-sds011
void novaPM::humidityCompensation(novaPMData &data, float humidity) {
  data.pm25 = data.pm25 / (1.0 + 0.48756 * pow((humidity / 100.0), 8.60068));
  data.pm10 = data.pm10 / (1.0 + 0.81559 * pow((humidity / 100.0), 5.83411));
}
