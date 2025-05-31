#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LDR_PIN A0
#define TRIG_PIN 2
#define ECHO_PIN 3
#define DHT_PIN 4
#define LED_PIN 5
#define BUZZER_PIN 6

DHT dht(DHT_PIN, DHT22);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  lcd.begin();
  lcd.backlight();
  dht.begin();
}

void loop() {
  // Lampu otomatis
  int light = analogRead(LDR_PIN);
  if (light < 500) digitalWrite(LED_PIN, HIGH);
  else digitalWrite(LED_PIN, LOW);

  // Deteksi orang
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2;
  if (distance > 0 && distance < 100) digitalWrite(BUZZER_PIN, HIGH);
  else digitalWrite(BUZZER_PIN, LOW);

  // Suhu & kelembapan
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  lcd.setCursor(0, 0);
  lcd.print("T: "); lcd.print(temp); lcd.print("C ");
  lcd.setCursor(0, 1);
  lcd.print("H: "); lcd.print(hum); lcd.print("% ");
  delay(1000);
}



//LDR → input cahaya A0
//Ultrasonic HC-SR04 → deteksi orang Trig D2 Echo D3
//DHT22 → suhu dan kelembapan D4
//LED → lampu otomatis D5
//Buzzer → orang di depan pintu D6
//LCD I2C → tampilkan suhu & kelembapan SDA A4 SCL A5


//LDR + Resistor	A0	Cek cahaya
//Ultrasonic TRIG	D5 (GPIO14)	Output ping Ultrasonic ECHO	D6 (GPIO12)	Input pantulan
//DHT22 → D2 (GPIO4)	Suhu & kelembaban
//LED → D7 (GPIO13)	Lampu otomatis
//Buzzer → D8 (GPIO15)	Bunyikan kalau ada orang
//LCD I2C → SDA	D2 (GPIO4)	Gabung dengan DHT22* SCL	D1 (GPIO5) *Gunakan Wire.begin(D2, D1) supaya bisa I2C tetap jalan bareng DHT

#define BLYNK_TEMPLATE_ID "YourTemplateID"
#define BLYNK_DEVICE_NAME "SmartHome"
#define BLYNK_AUTH_TOKEN "YourAuthToken"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char ssid[] = "WIFI_SSID";
char pass[] = "WIFI_PASS";

#define DHTPIN D4
#define LDR_PIN A0
#define TRIG_PIN D5
#define ECHO_PIN D6
#define LED_PIN D7
#define BUZZER_PIN D8

DHT dht(DHTPIN, DHT22);

BlynkTimer timer;

void sendSensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int light = analogRead(LDR_PIN);

  Blynk.virtualWrite(V0, t);      // suhu
  Blynk.virtualWrite(V1, h);      // kelembapan
  Blynk.virtualWrite(V2, light);  // cahaya

  if (light < 500) digitalWrite(LED_PIN, HIGH);
  else digitalWrite(LED_PIN, LOW);

  // Deteksi orang
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2;
  if (distance > 0 && distance < 100) digitalWrite(BUZZER_PIN, HIGH);
  else digitalWrite(BUZZER_PIN, LOW);
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  dht.begin();
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  timer.setInterval(2000L, sendSensor);
}

void loop() {
  Blynk.run();
  timer.run();
}
