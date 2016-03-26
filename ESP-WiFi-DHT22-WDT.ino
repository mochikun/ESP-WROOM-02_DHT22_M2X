#include <ESP8266WiFi.h>
#include "DHT.h"

#define DHTPIN 12     // what pin we're connected to
#define DHTTYPE DHT22   // DHT22

DHT dht(DHTPIN, DHTTYPE);

const char* ssid     = "WIFI-NO-SSID";
const char* password = "WIFI-NO-PASSWORD";

const char* host = "api-m2x.att.com";
const char* m2xKey = "M2X-NO-KEY";

// We now create a URI for the request
String tempurl = "/v2/devices/DEVICE_ID/streams/STREAM-NO-NAME/value";
String humurl = "/v2/devices/DEVICE_ID/streams/STREAM-NO-NAME/value";

void setup() {
  Serial.begin(115200);
  delay(1000);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("DHT22 test!");
  dht.begin();
}

void loop() {
  delay(2000);

  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  Serial.print("Requesting URL: ");
  Serial.println(tempurl);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  h = h + 8.0;
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C\n");

  // This will send the request to the server
  client.print(String("PUT ") + tempurl + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: curl/7.43.0\r\n" +
               "Accept: */*\r\n" +
               "X-M2X-KEY: " + m2xKey + "\r\n" +
               "Content-Type: application/json\r\n" +
               "Content-Length: 20\r\n\r\n" +
               "{ \"value\": \""+ String(t,2) +"\" }");

  delay(1000);

  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");

  client.print(String("PUT ") + humurl + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: curl/7.43.0\r\n" +
               "Accept: */*\r\n" +
               "X-M2X-KEY: " + m2xKey + "\r\n" +
               "Content-Type: application/json\r\n" +
               "Content-Length: 20\r\n\r\n" +
               "{ \"value\": \""+ String(h,2) +"\" }");
   delay(1000);

   // Read all the lines of the reply from server and print them to Serial
   while(client.available()){
     String line = client.readStringUntil('\r');
     Serial.print(line);
   }

   Serial.println();
   Serial.println("closing connection");

  // enter deep sleep
  ESP.deepSleep(300 * 1000 * 1000 , WAKE_RF_DEFAULT);
  delay(1000);

}
