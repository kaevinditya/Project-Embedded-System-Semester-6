🔥 PENDETEKSI KEBAKARAN (Arduino Uno)
Komponen:

Flame sensor

DHT22

Buzzer

LED

Servo (untuk buka pintu darurat)

Kode:
cpp
Salin
Edit
#include <DHT.h>
#include <Servo.h>

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define FLAME_PIN 3
#define BUZZER_PIN 5
#define LED_PIN 4
Servo pintu;

void setup() {
  Serial.begin(9600);
  dht.begin();
  pintu.attach(6);
  pinMode(FLAME_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int flame = digitalRead(FLAME_PIN);
  float suhu = dht.readTemperature();

  Serial.print("Api terdeteksi: "); Serial.println(flame == LOW ? "YA" : "TIDAK");
  Serial.print("Suhu: "); Serial.println(suhu);

  if (flame == LOW || suhu > 50) {
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
    pintu.write(90); // buka pintu
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    pintu.write(0);  // tutup pintu
  }

  delay(1000);
}