
#define GAS_PIN 4
#define PIR_PIN 2
#define TRIG_PIN 8
#define ECHO_PIN 9
#define BUZZER 10
#define LED_PIN 11

const float DISTANCE_THRESHOLD = 100.0;

void setup() {
  Serial.begin(9600);

  pinMode(GAS_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(BUZZER, LOW);
  digitalWrite(LED_PIN, LOW);

  Serial.println("SMART HOME SAFETY SYSTEM");
  Serial.println("------------------------");
}

void loop() {
  // Read gas Logic Toggle and PIR sensor
  bool gasDetected = (digitalRead(GAS_PIN) == HIGH);
  bool motionDetected = (digitalRead(PIR_PIN) == HIGH);

  // Trigger ultrasonic sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo with timeout
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000UL);

  float distance = duration * 0.0343 / 2.0;

  // Detect an object within 100 cm
  bool objectDetected =
      (duration > 0 && distance <= DISTANCE_THRESHOLD);

  // Any sensor detection activates both outputs
  bool alert = gasDetected || motionDetected || objectDetected;

  digitalWrite(LED_PIN, alert ? HIGH : LOW);
  digitalWrite(BUZZER, alert ? HIGH : LOW);

  // Display gas status
  Serial.print("Gas: ");
  Serial.println(gasDetected ? "DETECTED" : "NOT DETECTED");

  // Display motion status
  Serial.print("Motion: ");
  Serial.println(motionDetected ? "DETECTED" : "NOT DETECTED");

  // Display distance
  Serial.print("Distance: ");
  if (duration == 0) {
    Serial.println("NO ECHO");
  } else {
    Serial.print(distance, 1);
    Serial.println(" cm");
  }

  // Display object status
  Serial.print("Object: ");
  Serial.println(objectDetected ? "DETECTED" : "NOT DETECTED");

  // Display output status
  Serial.print("LED: ");
  Serial.println(alert ? "ON" : "OFF");

  Serial.print("BUZZER: ");
  Serial.println(alert ? "ON" : "OFF");

  Serial.print("STATUS: ");
  Serial.println(alert ? "UNSAFE" : "SYSTEM SAFE");

  Serial.println("------------------------");
  delay(500);
}
