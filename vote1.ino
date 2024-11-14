#include <Adafruit_Fingerprint.h>
#include <Wire.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial);

const int regButtonPin = 7;    // Button to start registration
int fingerprintsRegistered = 0;
int maxFingerprints = 3;

void setup() {
  Serial.begin(9600);
  finger.begin(57600);
  lcd.begin(16, 2);

  if (finger.verifyPassword()) {
    lcd.print("Sensor connected");
  } else {
    lcd.print("Sensor error");
    while (1);
  }
  
  pinMode(regButtonPin, INPUT_PULLUP);  // Set registration button
  lcd.clear();
}

void registerFingerprint() {
  lcd.clear();
  lcd.print("Place finger");

  while (finger.getImage() != FINGERPRINT_OK);
  if (finger.image2Tz(1) != FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("Error, try again");
    delay(2000);
    return;
  }

  if (finger.fingerFastSearch() == FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("Fingerprint exists");
    delay(2000);
    return;
  }

  lcd.clear();
  lcd.print("Remove finger");
  delay(2000);

  lcd.clear();
  lcd.print("Place again");
  while (finger.getImage() != FINGERPRINT_OK);
  if (finger.image2Tz(2) != FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("Error, try again");
    delay(2000);
    return;
  }

  if (finger.createModel() != FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("Error matching");
    delay(2000);
    return;
  }

  if (finger.storeModel(fingerprintsRegistered + 1) == FINGERPRINT_OK) {
    fingerprintsRegistered++;
    lcd.clear();
    lcd.print("Registered ");
    lcd.print(fingerprintsRegistered);
    delay(2000);
  } else {
    lcd.clear();
    lcd.print("Storage failed");
    delay(2000);
  }
}

void loop() {
  if (digitalRead(regButtonPin) == LOW) {
    if (fingerprintsRegistered < maxFingerprints) {
      registerFingerprint();
    } else {
      lcd.clear();
      lcd.print("Max registered");
      delay(2000);
    }
  }
}