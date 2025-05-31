// 🌾 SMART FARMING (Arduino Uno)
// Komponen:

// Pompa (via relay)

// Soil sensor

// Rain drop

// DHT22

// LDR

// LED (indikator tanah kering)


#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define SOIL_PIN A0
#define RAIN_PIN A1
#define LDR_PIN A2
#define RELAY_PIN 7
#define LED_PIN 4

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int soil = analogRead(SOIL_PIN);
  int rain = analogRead(RAIN_PIN);
  int cahaya = analogRead(LDR_PIN);
  float suhu = dht.readTemperature();

  Serial.print("Soil: "); Serial.println(soil);
  Serial.print("Rain: "); Serial.println(rain);
  Serial.print("Cahaya: "); Serial.println(cahaya);
  Serial.print("Suhu: "); Serial.println(suhu);

  // Siram jika tanah kering dan tidak hujan
  if (soil < 400 && rain > 600) {
    digitalWrite(RELAY_PIN, HIGH); // nyalakan pompa
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
  }

  delay(2000);
}