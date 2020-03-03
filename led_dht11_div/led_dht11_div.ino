#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Ticker.h>
#include "DHT.h"

// Set these to run example.
#define FIREBASE_HOST "webhome-5e814.firebaseio.com"
#define FIREBASE_AUTH "3gAnSCcSxTUGT8Wy1YhIJ61Im8IIYNtjUt0F8OKM"
#define WIFI_SSID "CACHACA"
#define WIFI_PASSWORD ""

#define LAMP_PIN D3
#define PRESENCE_PIN D4
#define DHT_PIN D1
#define DHTTYPE DHT11
// Publique a cada 5 min
#define PUBLISH_INTERVAL 1000*60*5

DHT dht(DHT_PIN, DHTTYPE);
Ticker ticker;
bool publishNewState = true;

void publish(){
  publishNewState = true;
}

void setupPins(){

  pinMode(LAMP_PIN, OUTPUT);
  digitalWrite(LAMP_PIN, LOW);
  
  pinMode(PRESENCE_PIN, INPUT);

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
}

void setupFirebase(){
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setBool("lamp", false);
  Firebase.setBool("presence", false);
}

void setup() {
  Serial.begin(9600);

  setupPins();
  setupWifi();    

  setupFirebase();

  // Registra o ticker para publicar de tempos em tempos
  ticker.attach_ms(PUBLISH_INTERVAL, publish);
}

void loop() {

  // Apenas publique quando passar o tempo determinado
  if(publishNewState){
    Serial.println("Publish new State");
    // Obtem os dados do sensor DHT 
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    if(!isnan(humidity) && !isnan(temperature)){
      // Manda para o firebase
      Firebase.setFloat("temperature", temperature);
      Firebase.setFloat("humidity", humidity);    
      Serial.print("Humidity: ");  Serial.print(humidity);
      Serial.print("%  Temperature: ");  Serial.print(temperature);  Serial.println("°C ");
      publishNewState = false;
    }else{
      Serial.println("Error Publishing");
    }
  }

  // Verifica o valor do sensor de presença
  // LOW sem movimento
  // HIGH com movimento
  int presence = digitalRead(PRESENCE_PIN);  
  Firebase.setBool("presence", presence == LOW);

  // Verifica o valor da lampada no firebase 
  bool lampValue = Firebase.getBool("lamp");
  digitalWrite(LAMP_PIN, lampValue ? HIGH : LOW);
  Serial.print(lampValue ? HIGH : LOW);
  delay(200);

  if (Firebase.failed()) {
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error());
      Firebase.setBool("espConnected", LOW);
      return;
  }else{
      Firebase.setBool("espConnected", HIGH);
  }

}
