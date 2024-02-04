#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
const int buzzerPin = 8;
int sensorPin = A0;
int sensorValue = 0;
int threshold = 400;
unsigned long startTime = 0;
unsigned long signalDuration = 0;

const int startButtonPin = 9;
const int stopButtonPin = 10;
const int muteButtonPin = 11;

bool circuitRunning = false;
bool buzzerMuted = false;

void setup() {
  lcd.begin(16, 2);
    lcd.print("   Welcome  ");
  pinMode(buzzerPin, OUTPUT);
  pinMode(startButtonPin, INPUT_PULLUP);
  pinMode(stopButtonPin, INPUT_PULLUP);
  pinMode(muteButtonPin, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(startButtonPin) == LOW && !circuitRunning) {
    circuitRunning = true;
    lcd.clear();
    lcd.print("   Welcome  ");

    delay(1000); // Optional delay for better readability
  }

  if (digitalRead(stopButtonPin) == LOW) {
    circuitRunning = false;
    noTone(buzzerPin);
    lcd.clear();
    lcd.print("  no detection  ");
  }

  if (digitalRead(muteButtonPin) == LOW) {
    // Toggle the mute state only once per button press
    delay(50);  // Debouncing delay
    if (digitalRead(muteButtonPin) == LOW) {
      buzzerMuted = !buzzerMuted;
      // If muted, immediately stop the buzzer
      if (buzzerMuted) {
        noTone(buzzerPin);
            lcd.setCursor(0, 1);
            lcd.print("   muted  ");
      }
      delay(500);  // Optional delay to prevent rapid toggling
    }
  }

  if (circuitRunning) {
    sensorValue = analogRead(sensorPin);

    if (sensorValue > threshold) {
      if (startTime == 0) {
        startTime = millis();
      }
    } else {
      if (startTime != 0) {
        signalDuration = millis() - startTime;
        startTime = 0;
        Serial.print("Signal Duration: ");
        Serial.println(signalDuration);
          if (signalDuration > 500 && signalDuration < 2000) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Smoke detected  ");
            tone(buzzerPin, 500);
          } else if (signalDuration > 2000 && signalDuration < 5000) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Intrusion detect");
            tone(buzzerPin, 500);
          } else if (signalDuration > 5000 && signalDuration < 10000) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Motion detected ");
            tone(buzzerPin, 500);
          }
        }
      }
    }
  }