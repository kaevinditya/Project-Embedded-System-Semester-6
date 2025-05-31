#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define MQ2_PIN A0
#define FLAME_PIN 2
#define DHT_PIN 3
#define LED_PIN 4
#define BUZZER_PIN 5

DHT dht(DHT_PIN, DHT22);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(FLAME_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  lcd.begin(); lcd.backlight();
  dht.begin();
}

void loop() {
  int gas = analogRead(MQ2_PIN);
  bool flame = digitalRead(FLAME_PIN) == LOW; // aktif LOW
  float suhu = dht.readTemperature();

  bool bahaya = false;
  if (gas > 400 || suhu > 50 || flame) bahaya = true;

  digitalWrite(LED_PIN, bahaya);
  digitalWrite(BUZZER_PIN, bahaya);

  lcd.setCursor(0, 0);
  lcd.print("T: "); lcd.print(suhu); lcd.print("C   ");
  lcd.setCursor(0, 1);
  if (bahaya) lcd.print("BAHAYA!!     ");
  else lcd.print("Aman          ");

  delay(1000);
}


//MQ-2 → asap/gas A0
//DHT22 → suhu D2
//Flame Sensor (digital) → deteksi nyala api D3
//LED Merah → bahaya D4
//Buzzer → alarm D5
//LCD I2C → suhu & status SDA A4, SCL A5

//MQ-2 → A0	Deteksi gas/asap
//DHT22 → D2 (GPIO4)	Suhu
//Flame Sensor (digital) → D3 (GPIO0)	LOW = api terdeteksi
//LED Merah → D7 (GPIO13)	Bahaya nyala
//Buzzer → D8 (GPIO15)	Alarm kebakaran
//LCD I2C → SDA	D2 (GPIO4) SCL	D1 (GPIO5)


#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define MQ2_PIN A0
#define FLAME_PIN 2
#define DHT_PIN 3
#define LED_PIN 4
#define BUZZER_PIN 5

DHT dht(DHT_PIN, DHT22);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(FLAME_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  lcd.begin(); lcd.backlight();
  dht.begin();
}

void loop() {
  int gas = analogRead(MQ2_PIN);
  bool flame = digitalRead(FLAME_PIN) == LOW; // aktif LOW
  float suhu = dht.readTemperature();

  bool bahaya = false;
  if (gas > 400 || suhu > 50 || flame) bahaya = true;

  digitalWrite(LED_PIN, bahaya);
  digitalWrite(BUZZER_PIN, bahaya);

  lcd.setCursor(0, 0);
  lcd.print("T: "); lcd.print(suhu); lcd.print("C   ");
  lcd.setCursor(0, 1);
  if (bahaya) lcd.print("BAHAYA!!     ");
  else lcd.print("Aman          ");

  delay(1000);
}

