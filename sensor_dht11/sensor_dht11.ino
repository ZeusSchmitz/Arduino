#include <Firebase.h>
#include <FirebaseArduino.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>

/* Code written by Rishi Tiwari
 *  Website: https://tricksumo.com
 *  https://youtu.be/hPv9uX3rUWc
 *  https://youtu.be/pA_Rfop7kDc
 *  https://gist.github.com/TrickSumo/f1aecd17972aa3b6c0668750c3309714
 *  
 *  Libraries:- Arduino firebase Master  https://github.com/FirebaseExtended/firebase-arduino
 *              Ardunio Json  https://github.com/bblanchon/ArduinoJson/tree/5.x
 *              DHT11 by adafruit  https://github.com/adafruit/DHT-sensor-library
 *              adafruit universal senor  https://github.com/adafruit/Adafruit_Sensor
 */


#include <ESP8266WiFi.h>
#include <DHT.h>   

#define DHTTYPE DHT11   // Sensor DHT 11  
#define dht_dpin D1
#define FIREBASE_HOST "webhome-5e814.firebaseio.com"
#define FIREBASE_AUTH "3gAnSCcSxTUGT8Wy1YhIJ61Im8IIYNtjUt0F8OKM"
#define WIFI_SSID "FREE"
#define WIFI_PASSWORD ""
DHT dht(dht_dpin, DHTTYPE);      

void setup() {

  dht.begin();    
  Serial.begin(9600);

  Serial.println("Humidity and temperature \n\n");  
           
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                            //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                              // connect to firebase
                                                             //Start reading dht sensor
}

void loop() {   
  float h = dht.readHumidity();                                              // Reading humidity
  float t = dht.readTemperature();                                           // Reading temperature
    
  if (isnan(h) || isnan(t)) {                                                // Check if any reads failed and exit early (to try again).
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");  Serial.print(h);
  String fireHumid = String(h) + String("%");                                         //convert integer humidity to string humidity 
  Serial.print("%  Temperature: ");  Serial.print(t);  Serial.println("°C ");
  String fireTemp = String(t) + String("°C");                                                     //convert integer temperature to string temperature
  delay(4000);
  
  Firebase.setFloat("/DHT11/Humidity", h);                                  //setup path and send readings
  Firebase.setFloat("/DHT11/Temperature", t);                                //setup path and send readings

  if (Firebase.failed()) {
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error()); 
      return;
  }
}
