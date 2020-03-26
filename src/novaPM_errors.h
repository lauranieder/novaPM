/*
* novaPM Arduino Library
* Based on Hackair Arduino Library https://www.hackair.eu/docs/hackair-home-v1/arduino-library/
* Modified by Fragmentin --> fragment.in
*/

/** No error, everything is fine */
#define H_NO_ERROR 0

/** There was an error reading from the air quality sensor */
#define H_ERROR_SENSOR (1 << 0)

/**
 * There was an error reading from the sensor at least once
 * but the reading should be correct
 */
#define H_ERROR_SENSOR_ONCE (1 << 1)

/** There was an error reading from the humidity sensor */
#define H_ERROR_HUMIDITY (1 << 2)

/** Something else went wrong */
#define H_ERROR_UNKNOWN (1 << 3)
