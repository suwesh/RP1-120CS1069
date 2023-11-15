#include <WiFi.h>
#include <ThingSpeak.h>
#include <RTClib.h>
#include <string.h>
RTC_DS1307 rtc;
String Timestamp;
String irid = "IR1";
//connecting to network
const char* ssid = "720G";
const char* password = "1234567890";

//thingspeak channel
unsigned long channelID = 2339734;
const char* writeAPIKey = "FHS29BP22IIQDV8V";

WiFiClient client;

//ir sensor
const int irsensor = A0;// connect IR sensor module to A0
const int esp32pin = 15; // Connect the output of IR sensor to GPIO 15 on ESP32

bool initialDataSent = false;

void setup() {
  Serial.begin(115200);
  delay(10);
  //connect to wifi
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to wifi...");
  }
  Serial.println("Connected to wifi.");

  pinMode(esp32pin, INPUT);
  //ir sensor pin input mode
  pinMode(irsensor, INPUT);

  //initialize thingspeak
  ThingSpeak.begin(client);
}

void loop() {
  // read irsensor value
  String SlotID = irid;
  int irstatus = digitalRead(irsensor);
  if (!initialDataSent) 
  {
    int Availability = irstatus;
    //timestamp
    rtc.begin();
    DateTime now = rtc.now();
    Timestamp = String(now.year())+"-"+String(now.month())+"-"+String(now.day())+"T"+String(now.hour())+":"+String(now.minute())+":"+String(now.second());
    //send data to thingspeak
    //ThingSpeak.writeField(channelID, 1, irid, writeAPIKey);
    //ThingSpeak.writeField(channelID, 2, irstatus, writeAPIKey);
    //ThingSpeak.writeField(channelID, 3, timestamp, writeAPIKey);
    //set thingspeak
    ThingSpeak.setField(1, SlotID);
    ThingSpeak.setField(2, Availability);
    ThingSpeak.setField(3, Timestamp);
    int httpCode = ThingSpeak.writeFields(channelID, writeAPIKey);
    if (httpCode == 200) {
      Serial.println("Data sent to ThingSpeak successfully");
    } else {
      Serial.print("Failed to send data. HTTP error code: ");
      Serial.println(httpCode);
    }
    initialDataSent = true;
  }
  delay(100);
  
  //read sensor data again
  int irstatusnew = digitalRead(irsensor);
  if(irstatusnew != irstatus)
  {
    int Availability = irstatus;
    //timestamp
    rtc.begin();
    DateTime now = rtc.now();
    Timestamp = String(now.year())+"-"+String(now.month())+"-"+String(now.day())+"T"+String(now.hour())+":"+String(now.minute())+":"+String(now.second());
    //send data to thingspeak
    //ThingSpeak.writeField(1, irid);
    //ThingSpeak.writeField(2, irstatusnew);
    //ThingSpeak.writeField(3, timestamp);
    ThingSpeak.setField(1, SlotID);
    ThingSpeak.setField(2, Availability);
    ThingSpeak.setField(3, Timestamp);
    int httpCode = ThingSpeak.writeFields(channelID, writeAPIKey);
    if (httpCode == 200) {
      Serial.println("Data sent to ThingSpeak successfully");
    } else {
      Serial.print("Failed to send data. HTTP error code: ");
      Serial.println(httpCode);
    }
  }
  delay(100);
}
