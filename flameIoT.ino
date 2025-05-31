
#define BLYNK_TEMPLATE_ID "YourTemplateID"
#define BLYNK_TEMPLATE_NAME "Kebakaran"
#define BLYNK_AUTH_TOKEN "YourAuthToken"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Servo.h>

char ssid[] = "YourWiFiSSID";
char pass[] = "YourWiFiPassword";

// DHT
#define DHTPIN D2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Flame Sensor
#define FLAME_PIN D1

// Output
#define BUZZER_PIN D5
#define LED_PIN D6
Servo pintu;

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  dht.begin();
  pintu.attach(D4);
  pinMode(FLAME_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  Blynk.run();

  float suhu = dht.readTemperature();
  int flame = digitalRead(FLAME_PIN); // LOW = api terdeteksi

  Blynk.virtualWrite(V0, suhu);
  Blynk.virtualWrite(V1, flame == LOW ? "BAHAYA API" : "AMAN");

  if (flame == LOW || suhu > 50) {
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
    pintu.write(90);
    Blynk.virtualWrite(V2, "Pintu Darurat Terbuka");
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    pintu.write(0);
    Blynk.virtualWrite(V2, "Pintu Tertutup");
  }

  delay(2000);
}


// PENDETEKSI KEBAKARAN — ESP8266 + Blynk
// Komponen:
// Flame Sensor (digital)

// DHT22

// LED

// Buzzer

// Servo (pintu darurat)

// 🔌 Pin Mapping:
// Sensor/Output	Pin ESP8266
// Flame	D1
// DHT22	D2
// Buzzer	D5
// LED	D6
// Servo	D4

// 🧠 Logika:
// Jika flame terdeteksi ATAU suhu tinggi → aktifkan alarm (buzzer & LED), buka pintu (servo).

// Kirim status ke Blynk.

// Blynk Virtual Pins:
// V0: Suhu

// V1: Flame status

// V2: Status pintu darurat
