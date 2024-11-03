#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "DHT.h"
#include "variables.h"

DHT dht(DHT_PIN, DHT_TYPE);

WiFiClientSecure client;

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(KY037_PIN, INPUT);

  Serial.print("Connecting to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());

  Serial.println("Setup done");
}

void httpUpdate(float temp, float humid, float noise) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Unable to send http request. Not connected.");
    return;
  }

  String data = "{\"maquina_id\":\"1\",";
  data += "\"temperatura\":\""+String(temp)+"\",";
  data += "\"umidade\":\""+String(humid)+"\",";
  data += "\"ruido\":\""+String(noise)+"\"}";

  client.setInsecure();
  if (client.connect(SERVER_NAME, SERVER_PORT)) {
    client.println("POST "+String(SERVER_PATH)+" HTTP/1.1");
    client.println("Host: " + String(SERVER_NAME));
    client.println("Content-Type: application/json");
    client.println("Content-Length: "+String(data.length()));
    client.println("Connection: close");
    client.println();
    client.println(data);

    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("headers received");
        break;
      }
    }
    // if there are incoming bytes available
    // from the server, read them and print them:
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }

    client.stop();
  } else {
    Serial.println("Error: Could not connect to server");
  }
}

void loop() {
  float temp = dht.readTemperature(false, false);
  float humid = dht.readHumidity(false);
  float noise = analogRead(KY037_PIN);

  httpUpdate(temp, humid, noise);

  Serial.println("DHT Temperature: " + String(temp) + "C");
  Serial.println("DHT Humidity: " + String(humid) + "%");
  Serial.println("Noise level: " + String(noise));
  Serial.println("");

  delay(DELAY_MS);
}
