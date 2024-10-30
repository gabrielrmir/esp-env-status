#include <WiFi.h>
#include "DHT.h"
#include "variables.h"

DHT dht(DHT_PIN, DHT_TYPE);

WiFiClient client;

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

void httpUpdate() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Unable to send http request. Not connected.");
    return;
  }

  int temp = dht.readTemperature(false, false);
  int humid = dht.readHumidity(false);
  int noise = analogRead(KY037_PIN);

  String data = "{\"maquina_id\":\"1\",";
  data += "\"temperatura\":\""+String(temp)+"\",";
  data += "\"umidade\":\""+String(humid)+"\",";
  data += "\"ruido\":\""+String(noise)+"\"}";

  if (client.connect(SERVER_NAME, SERVER_PORT)) {
    client.println("POST "+String(SERVER_PATH)+" HTTP/1.1");
    client.println("Host: " + String(SERVER_NAME)+":"+String(SERVER_PORT));
    client.println("Content-Type: application/json");
    client.println("Content-Length: "+String(data.length()));
    client.println("Connection: close");
    client.println();
    client.println(data);
  } else {
    Serial.println("Error: Could not connect to server");
  }
}

void loop() {
  httpUpdate();

  Serial.println("DHT Temperature: " + String(temp) + "C");
  Serial.println("DHT Humidity: " + String(humid) + "%");
  Serial.println("Noise level: " + String(analogRead(4)));
  Serial.println("===========");

  delay(DELAY_MS);
}