#define LDR_PIN A0
#define PIR_PIN 2
#define LED_JALAN 3

int thresholdLDR = 500; // Sesuaikan nilai ini sesuai kondisi cahaya ruangan lo

void setup() {
  pinMode(LDR_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_JALAN, OUTPUT);
  Serial.begin(9600);
  Serial.println("EcoLight Aktif");
}

void loop() {
  int cahaya = analogRead(LDR_PIN);
  int gerakan = digitalRead(PIR_PIN);

  Serial.print("Cahaya: "); Serial.print(cahaya);
  Serial.print(" | Gerakan: "); Serial.println(gerakan);

  if (cahaya < thresholdLDR || gerakan == HIGH) {
    digitalWrite(LED_JALAN, HIGH);
  } else {
    digitalWrite(LED_JALAN, LOW);
  }

  delay(200);
}
