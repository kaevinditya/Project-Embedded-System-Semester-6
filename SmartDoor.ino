// 🏠 SMART HOME (Arduino Uno)
// Komponen:

// RFID RC522

// Servo (pintu)

// DHT22 (suhu)

// LDR (sensor cahaya)

// Relay (lampu)

// Buzzer

// LED

#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <DHT.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN);

Servo pintu;
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define RELAY_PIN 7
#define BUZZER_PIN 5
#define LED_PIN 4
#define LDR_PIN A0

String kartuTerdaftar = "AB CD EF 12";  // ganti dengan UID kartumu

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  dht.begin();
  pintu.attach(6); // PWM
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LDR_PIN, INPUT);
}

void loop() {
  // === RFID ===
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String uid = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      uid += String(rfid.uid.uidByte[i], HEX);
    }
    uid.toUpperCase();

    if (uid == kartuTerdaftar) {
      Serial.println("Akses diterima");
      pintu.write(90); // buka pintu
      digitalWrite(BUZZER_PIN, HIGH); delay(200);
      digitalWrite(BUZZER_PIN, LOW);
      delay(3000);
      pintu.write(0);  // tutup pintu
    } else {
      Serial.println("Kartu tidak dikenal");
      digitalWrite(LED_PIN, HIGH);
      delay(1000);
      digitalWrite(LED_PIN, LOW);
    }
    rfid.PICC_HaltA();
  }

  // === DHT22 ===
  float suhu = dht.readTemperature();
  Serial.print("Suhu: "); Serial.println(suhu);

  // === LDR ===
  int cahaya = analogRead(LDR_PIN);
  Serial.print("Cahaya: "); Serial.println(cahaya);
  if (cahaya < 500) {
    digitalWrite(RELAY_PIN, HIGH); // nyalakan lampu
  } else {
    digitalWrite(RELAY_PIN, LOW);
  }

  delay(2000);
}