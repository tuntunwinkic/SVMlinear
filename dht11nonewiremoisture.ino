#include <ESP8266WiFi.h>
#include "DHT.h"

#define DHTPIN D8 // what digital pin we're connected to

#define DHTTYPE DHT11   // DHT 11  (AM2302), AM2321
#include <OneWire.h>
#include <DallasTemperature.h>
/********************************************************************/
// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 14  //connect to D5 pin of Wemos
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
/********************************************************************/
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);


// Wi-Fi Settings
const char* ssid = "zzwifi"; // your wireless network name (SSID)
const char* password = "123456789"; // your Wi-Fi network password

DHT dht1(DHTPIN, DHTTYPE);

DHT dht2(D9, DHTTYPE);

WiFiClient client;

// ThingSpeak Settings
const int channelID = 656018;
String writeAPIKey = "GXIZD2XSS09EQ38K"; // write API key for your ThingSpeak Channel
const char* server = "api.thingspeak.com";
const int postingInterval = 300 * 1000; // post data every x seconds

void setup() {
  Serial.begin(19200);

  Serial.println("Starting...");
  // Start up the library
  sensors.begin(); //onewire sensors
  dht1.begin();
  dht2.begin();
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



    // Measure Analog Input (A0)
    float h1 = dht1.readHumidity();
    float t1 = dht1.readTemperature();

    float h2 = dht2.readHumidity();
    float t2 = dht2.readTemperature();
    Serial.println("From Sensor 1");
    Serial.println(h1);
    Serial.println(t1);

    Serial.println("From Sensor 2");
    Serial.println(h2);
    Serial.println(t2);


    Serial.print("Requesting temperatures...");
    sensors.requestTemperatures(); // Send the command to get temperature readings
    Serial.println("DONE");
    /********************************************************************/
    //int deviceCount=0;
    //deviceCount = sensors.getDeviceCount();

    // Serial.print(deviceCount);
    float soilone = sensors.getTempCByIndex(0);
    float soiltwo = sensors.getTempCByIndex(1);
    //sensors.getTempCByIndex(1);
    Serial.print("Soil Temperature Sensor one from One Wire  is : ");
    Serial.println(soilone);
    Serial.print("Soil Temperature Sensor two from One Wire  is : ");
    Serial.println(soiltwo);
    // Why "byIndex"?
    // You can have more than one DS18B20 on the same bus.
    // 0 refers to the first IC on the wire

    // read the input on analog pin 0:
    int soilmoisture = analogRead(A0);
    // print out the value you read:
    Serial.print("Soil Moisture Sensor reading is");
    Serial.println(soilmoisture);



    // Construct API request body
    String body = "&field1=";
    body += String(t1);
    body += "&field2=";
    body += String(h1);
    body += "&field3=";
    body += String(t2);
    body += "&field4=";
    body += String(h2);
    body += "&field5=";
    body += String(soilone);
    body += "&field6=";
    body += String(soiltwo);
    body += "&field7=";
    body += String(soilmoisture);





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
