#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Ticker.h>
#include "DHT.h"

// Set these to run example.
#define FIREBASE_HOST "webhome-5e814.firebaseio.com"
#define FIREBASE_AUTH "3gAnSCcSxTUGT8Wy1YhIJ61Im8IIYNtjUt0F8OKM"
#define WIFI_SSID "CACHACA"
#define WIFI_PASSWORD ""

#define LED_PIN D1
#define DHT_PIN D4
#define DHTTYPE DHT11
// Publique a cada 5 min
#define PUBLISH_INTERVAL 5000

DHT dht(DHT_PIN, DHTTYPE);
Ticker ticker;
bool publishNewState = true;

void publish(){
  publishNewState = true;
}

void setupPins(){
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  pinMode(DHT_PIN, INPUT);

  dht.begin();  
}

void setupWifi(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  Serial.println(WIFI_SSID);
}

void setupFirebase(){
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.set("LED_STATUS",0);
}

void setup() {
  Serial.begin(9600);

  setupPins();
  setupWifi();    

  setupFirebase();
}
int n = 0;
void loop() {

  // set value    
  n=Firebase.getInt("LED_STATUS"); 
  // handle error  
  if (n==1) {  
      Serial.print("LED is ON");  
      digitalWrite(LED_PIN,HIGH);  
      Serial.println(Firebase.error());    
      return;  
       delay(100);  
  }  
 else{  
   Serial.print("LED is OFF");  
   digitalWrite(LED_PIN,LOW);  
 }  
  delay(200);
}
