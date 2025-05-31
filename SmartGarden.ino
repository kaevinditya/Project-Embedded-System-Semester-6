#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHT_PIN 2
#define LDR_PIN A0
#define MQ2_PIN A1
#define GREEN_LED 3
#define YELLOW_LED 4

DHT dht(DHT_PIN, DHT22);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  lcd.begin(); lcd.backlight();
  dht.begin();
}

void loop() {
  float suhu = dht.readTemperature();
  float hum = dht.readHumidity();
  int cahaya = analogRead(LDR_PIN);
  int gas = analogRead(MQ2_PIN);

  // Logika LED
  if (cahaya < 400) {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
  } else {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
  }

  lcd.setCursor(0, 0);
  lcd.print("T:"); lcd.print(suhu);
  lcd.print(" H:"); lcd.print(hum);

  lcd.setCursor(0, 1);
  lcd.print("Cahaya:"); lcd.print(cahaya);

  delay(1000);
}



//DHT22 → suhu & kelembaban D2 
//LDR → intensitas cahaya A0 
//MQ-2 → gas berbahaya A1
//LED Hijau → kondisi oke D3
//LED Kuning → kurang cahaya D4
//LCD I2C → status pertanian SDA A4 SCL A5


//DHT22 → D2 (GPIO4)	Deteksi suhu & kelembaban
//LDR → A0	Deteksi cahaya
//MQ-2 → Gak dipakai (opsional)	Bisa diganti LDR kalau A0 dipakai
//LED Hijau → D6 (GPIO12)	Kondisi ideal
//LED Kuning → D7 (GPIO13)	Kurang cahaya
//LCD I2C → SDA	D2 (GPIO4) SCL	D1 (GPIO5)


#define BLYNK_TEMPLATE_ID "YourTemplateID"
#define BLYNK_DEVICE_NAME "SmartFarm"
#define BLYNK_AUTH_TOKEN "YourAuthToken"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char ssid[] = "WIFI_SSID";
char pass[] = "WIFI_PASS";

#define DHT_PIN D1
#define LDR_PIN A0
#define MQ2_PIN A0  // Pakai input bergantian (atau gunakan eksternal ADC)
#define GREEN_LED D2
#define YELLOW_LED D3

DHT dht(DHT_PIN, DHT22);
BlynkTimer timer;

void kirimData() {
  float suhu = dht.readTemperature();
  float hum = dht.readHumidity();
  int cahaya = analogRead(LDR_PIN);
  int gas = analogRead(MQ2_PIN);

  Blynk.virtualWrite(V0, suhu);
  Blynk.virtualWrite(V1, hum);
  Blynk.virtualWrite(V2, cahaya);
  Blynk.virtualWrite(V3, gas);

  if (cahaya < 400) {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
  } else {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
  }
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  dht.begin();
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  timer.setInterval(2000L, kirimData);
}

void loop() {
  Blynk.run();
  timer.run();
}
