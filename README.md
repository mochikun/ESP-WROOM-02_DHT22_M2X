ESP-WROOM-02_DHT22
=============
_This software is released under the MIT License, see LICENSE.txt._ 

# Description  
This program is Arduino sketch for ESP-WROOM-02 with DHT22.  
DHT22 temp and hum data are uploaded to AT&T M2X.  
  
Plese input your environment information in ESP-WiFi-HDT22-WDT.ino  
  
const char* ssid     = "WIFI-NO-SSID";  
const char* password = "WIFI-NO-PASSWORD";  
const char* m2xKey = "M2X-NO-KEY";  
// We now create a URI for the request  
String tempurl = "/v2/devices/DEVICE_ID/streams/STREAM-NO-NAME/value";  
String humurl = "/v2/devices/DEVICE_ID/streams/STREAM-NO-NAME/value";  
