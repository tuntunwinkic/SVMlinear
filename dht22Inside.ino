#include <ESP8266WiFi.h>
#include "DHT.h"

#define DHTPIN D8     // what digital pin we're connected to

#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

// Wi-Fi Settings
const char* ssid = "zzone"; // your wireless network name (SSID)
const char* password = "123456789"; // your Wi-Fi network password

DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;

// ThingSpeak Settings
const int channelID = 768643;
String writeAPIKey = "4Y1AM0EW3R2NNC6D"; // write API key for your ThingSpeak Channel
const char* server = "api.thingspeak.com";
const int postingInterval = 300 * 1000; // post data every x seconds

void setup() {
  Serial.begin(19200);

  Serial.println("DHT22 From inside Greenhouse Sensor");
  // Start up the library

  dht.begin();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);


  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  


}

void loop() {
  if (client.connect(server, 80)) {

    float h = dht.readHumidity();
    float t = dht.readTemperature();
    Serial.println(h);
    Serial.println(t);

    // Construct API request body
    String body = "&field1=";
    body += String(t);

    body += "&field2=";
    body += String(h);

    client.println("POST /update HTTP/1.1");
    client.println("Host: api.thingspeak.com");
    client.println("User-Agent: ESP8266 (nothans)/1.0");
    client.println("Connection: close");
    client.println("X-THINGSPEAKAPIKEY: " + writeAPIKey);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Content-Length: " + String(body.length()));
    client.println("");
    client.print(body);
  }



  // wait and then post again
  delay(postingInterval);

}
