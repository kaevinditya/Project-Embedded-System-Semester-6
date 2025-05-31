

#define BLYNK_TEMPLATE_ID "YourTemplateID"
#define BLYNK_TEMPLATE_NAME "SmartFarming"
#define BLYNK_AUTH_TOKEN "YourAuthToken"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char ssid[] = "YourWiFiSSID";
char pass[] = "YourWiFiPassword";

// DHT
#define DHTPIN D1
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Sensor Pin
#define SOIL_PIN A0
#define RAIN_PIN D2
#define RELAY_PIN D5
#define LED_PIN D6
#define LDR_PIN D7

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  dht.begin();

  pinMode(RAIN_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LDR_PIN, INPUT);
}

void loop() {
  Blynk.run();

  float suhu = dht.readTemperature();
  float hum = dht.readHumidity();
  int soil = analogRead(SOIL_PIN);
  int rain = digitalRead(RAIN_PIN); // LOW = hujan
  int cahaya = analogRead(LDR_PIN);

  Serial.print("Soil: "); Serial.println(soil);
  Serial.print("Rain: "); Serial.println(rain);
  Serial.print("Suhu: "); Serial.println(suhu);

  Blynk.virtualWrite(V0, suhu);
  Blynk.virtualWrite(V1, hum);
  Blynk.virtualWrite(V2, soil < 400 ? "Kering" : "Lembab");
  Blynk.virtualWrite(V3, rain == LOW ? "Hujan" : "Tidak Hujan");

  if (soil < 400 && rain == HIGH) {
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
    Blynk.virtualWrite(V4, "Pompa AKTIF");
  } else {
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    Blynk.virtualWrite(V4, "Pompa MATI");
  }

  delay(2000);
}

// SMART FARMING — ESP8266 + Blynk
// Komponen:
// Soil sensor

// Rain sensor

// DHT22

// Relay (nyambung ke pompa air)

// LED indikator tanah kering

// LDR (opsional)

// 🔌 Pin Mapping:
// Sensor/Output	Pin ESP8266 (NodeMCU)
// DHT22	D1
// Soil sensor	A0
// Rain sensor	D2
// Relay (pompa)	D5
// LED indikator	D6
// LDR (opsional)	D7

// 🧠 Logika:
// Jika tanah kering (soil < threshold) dan tidak hujan, pompa menyala.

// Kirim suhu, kelembapan, status tanah & hujan ke Blynk.

// Blynk Virtual Pins:
// V0: Suhu

// V1: Kelembapan

// V2: Status tanah

// V3: Status hujan

// V4: Status pompa