#ifndef VARIABLES_H
#define VARIABLES_H

// Sensor information
#define DHT_PIN 19
#define DHT_TYPE DHT11

#define KY037_PIN 4

// Wifi settings
#define WIFI_SSID "ssid"
#define WIFI_PASSWORD "password"

// Server connection
#define SERVER_NAME "192.168.1.222"
#define SERVER_PATH "/api/example/path"
#define SERVER_PORT 80

// How often the board sends its
// data to the server in milliseconds
#define DELAY_MS 10000

#endif
