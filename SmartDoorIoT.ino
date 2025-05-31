

#define BLYNK_TEMPLATE_ID "YourTemplateID"
#define BLYNK_TEMPLATE_NAME "SmartHome"
#define BLYNK_AUTH_TOKEN "YourAuthToken"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SPI.h>
#include <MFRC522.h>
#include <DHT.h>
#include <Servo.h>

// WiFi
char ssid[] = "YourWiFiSSID";
char pass[] = "YourWiFiPassword";

// RFID
#define RST_PIN D1
#define SS_PIN D2
MFRC522 rfid(SS_PIN, RST_PIN);

// DHT
#define DHTPIN D3
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// LDR
#define LDR_PIN A0

// Relay & Buzzer
#define RELAY_PIN D5
#define BUZZER_PIN D6
#define LED_PIN D7

// Servo
Servo pintu;

String kartuTerdaftar = "ABCDEF12"; // Ganti dengan UID RFID kamu

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  dht.begin();
  pintu.attach(D4);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();

  // === RFID ===
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String uid = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      uid += String(rfid.uid.uidByte[i], HEX);
    }
    uid.toUpperCase();

    if (uid == kartuTerdaftar) {
      Serial.println("Akses diterima");
      pintu.write(90);
      digitalWrite(BUZZER_PIN, HIGH);
      Blynk.virtualWrite(V2, "Pintu Terbuka");
      delay(1000);
      digitalWrite(BUZZER_PIN, LOW);
      delay(3000);
      pintu.write(0);
      Blynk.virtualWrite(V2, "Pintu Tertutup");
    } else {
      digitalWrite(LED_PIN, HIGH);
      delay(1000);
      digitalWrite(LED_PIN, LOW);
    }
    rfid.PICC_HaltA();
  }

  // === DHT + LDR ===
  float suhu = dht.readTemperature();
  float hum = dht.readHumidity();
  int ldr = analogRead(LDR_PIN);

  Blynk.virtualWrite(V0, suhu);
  Blynk.virtualWrite(V1, hum);
  Blynk.virtualWrite(V4, ldr);

  // === Lampu Otomatis Berdasarkan Cahaya ===
  if (ldr < 500) {
    digitalWrite(RELAY_PIN, HIGH);
    Blynk.virtualWrite(V3, "Lampu HIDUP");
  } else {
    digitalWrite(RELAY_PIN, LOW);
    Blynk.virtualWrite(V3, "Lampu MATI");
  }

  delay(2000);
}

// SMART HOME — ESP8266 + Blynk
// Komponen:
// ESP8266 NodeMCU

// RFID RC522

// Servo motor (pintu)

// DHT22

// LDR

// Relay (lampu)

// LED

// Buzzer

// 🧠 Rencana Logika:
// Scan RFID → jika sesuai → buka pintu (servo), bunyi buzzer

// Sensor LDR → gelap → nyalakan relay (lampu)

// DHT22 dan status lainnya dikirim ke Blynk

// 🌐 Blynk Setup (Blynk IoT 2.0)
// Platform: https://blynk.cloud

// Gunakan Blynk Library terbaru

// Tambahkan widget:

// V0: Suhu

// V1: Kelembapan

// V2: Status pintu

// V3: Status lampu

// V4: Status cahaya (dari LDR)

// 📦 Library yang Dibutuhkan
// BlynkSimpleEsp8266.h

// SPI.h, MFRC522.h

// Servo.h

// DHT.h