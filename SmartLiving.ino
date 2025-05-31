**Smart Home**

**Tujuan alat:**
Sistem ini bertujuan untuk mengatur kenyamanan dan keamanan rumah secara otomatis dengan memanfaatkan sensor suhu & kelembapan (DHT22), deteksi gas (MQ-2), dan deteksi kehadiran (Ultrasonic). Output alat meliputi LED untuk indikasi pencahayaan, buzzer untuk alarm, dan servo untuk menggerakkan pintu otomatis.

**Sensor & Output yang Digunakan:**
1. **DHT22** - Mengukur suhu dan kelembapan ruangan.
2. **MQ-2** - Mendeteksi kebocoran gas atau asap.
3. **Ultrasonic** - Mendeteksi orang di depan pintu.
4. **LED** - Menyala jika terdeteksi gas.
5. **Buzzer** - Berbunyi saat terdeteksi gas atau ada orang.
6. **Servo** - Membuka pintu secara otomatis saat ada orang.

---

### 💡 Versi Arduino UNO

**Pin Sambungan:**
- DHT22 → D4
- MQ-2 → A0
- Ultrasonic TRIG → D2, ECHO → D3
- LED → D5
- Buzzer → D6
- Servo → D7

```cpp
#include <DHT.h>
#include <Servo.h>

#define DHTPIN 4
#define DHTTYPE DHT22
#define MQ_PIN A0
#define TRIG_PIN 2
#define ECHO_PIN 3
#define LED_PIN 5
#define BUZZER_PIN 6
#define SERVO_PIN 7

DHT dht(DHTPIN, DHTTYPE);
Servo myservo;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  dht.begin();
  myservo.attach(SERVO_PIN);
  myservo.write(0);
}

void loop() {
  // Sensor Gas
  int gas = analogRead(MQ_PIN);
  if (gas > 400) {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }

  // Sensor Ultrasonik
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2;
  if (distance > 0 && distance < 100) {
    myservo.write(90);  // buka pintu
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    myservo.write(0);   // tutup pintu
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(1000);
}
```

---

### 🌐 Versi ESP8266 + Blynk

**Pin Sambungan:**
- DHT22 → D2 (GPIO4)
- MQ-2 → A0
- Ultrasonic TRIG → D5 (GPIO14), ECHO → D6 (GPIO12)
- LED → D7 (GPIO13)
- Buzzer → D8 (GPIO15)
- Servo → D1 (GPIO5)

```cpp
#define BLYNK_TEMPLATE_ID "YourTemplateID"
#define BLYNK_DEVICE_NAME "SmartHome"
#define BLYNK_AUTH_TOKEN "YourAuthToken"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <Servo.h>

char ssid[] = "WIFI_SSID";
char pass[] = "WIFI_PASS";

#define DHTPIN D2
#define DHTTYPE DHT22
#define MQ_PIN A0
#define TRIG_PIN D5
#define ECHO_PIN D6
#define LED_PIN D7
#define BUZZER_PIN D8
#define SERVO_PIN D1

DHT dht(DHTPIN, DHTTYPE);
Servo myservo;
BlynkTimer timer;

void sendSensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int gas = analogRead(MQ_PIN);

  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
  Blynk.virtualWrite(V2, gas);

  if (gas > 400) {
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2;

  if (distance > 0 && distance < 100) {
    myservo.write(90);
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    myservo.write(0);
    digitalWrite(BUZZER_PIN, LOW);
  }
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  dht.begin();
  myservo.attach(SERVO_PIN);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  timer.setInterval(2000L, sendSensor);
  myservo.write(0);
}

void loop() {
  Blynk.run();
  timer.run();
}
```

---
