/*
* novaPM Arduino Library
* Based on Hackair Arduino Library https://www.hackair.eu/docs/hackair-home-v1/arduino-library/
* Modified by Fragmentin --> fragment.in
*/

#ifndef novaPM_h
#define novaPM_h

// Library version
#define novaPM_VERSION "1.0.0"


// Shield pins

#define PIN_SERIAL_TX 3
#define PIN_SERIAL_RX 2


#include "Arduino.h"
#include "novaPM_errors.h"
#include <SoftwareSerial.h>

/**
* Structure for storing & sending novaPM-related data to a
* server
*/
struct novaPMData {
float pm25;   /**< Amount of PM2.5 */
float pm10;   /**< Amount of PM1.0 */
int battery;  /**< Battery level */
int tamper;   /**< Tamper indicator */
int error;    /**< Error status */
};

class novaPM {
public:
    // Constructors
    novaPM();

    /**
     * Initialize the sensor (pin modes, buses, etc)
     */
    void begin();

    /**
     * Read data from the sensor.
     */
    void readData(novaPMData &data);

    /**
     * Read data from the sensor n times and return the average reading.
     */
    void readAverageData(novaPMData &data, uint8_t n);

    /**
     * Empties a novaPM data structure
     */
    void clearData(novaPMData &data);

    /**
     * Turn the sensor power on
     */
    void turnOn();

    /**
     * Turn the sensor power off
     */
    void turnOff();

    /**
     * Compensates the measurements for the given humidity
     */
    void humidityCompensation(novaPMData &data, float humidity);

private:
    long _lastTime;
    long _pulseDuration;
    unsigned char _buff[32];
};

#endif
