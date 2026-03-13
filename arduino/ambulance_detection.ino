#include <SPI.h>
#include <MFRC522.h>

/*
  Smart Ambulance Traffic Control System
  --------------------------------------
  This Arduino program controls a 3-road traffic signal system.

  Features:
  - Detects authorized RFID ambulance tag
  - Receives detection signal from Python (computer vision)
  - Gives green signal priority to the detected road
*/


// -----------------------------
// RFID Reader Configuration
// -----------------------------
#define SDA1 10
#define SDA2 9
#define SDA3 8
#define RST_PIN 7

MFRC522 rfid1(SDA1, RST_PIN);
MFRC522 rfid2(SDA2, RST_PIN);
MFRC522 rfid3(SDA3, RST_PIN);


// -----------------------------
// Traffic Light Pins
// -----------------------------

// Road 1
#define R1 2
#define Y1 3
#define G1 4

// Road 2
#define R2 5
#define Y2 6
#define G2 A0

// Road 3
#define R3 A1
#define Y3 A2
#define G3 A3


// -----------------------------
// Authorized Ambulance RFID UID
// -----------------------------
byte authorizedUID[4] = {0xD3, 0x13, 0x8B, 0xF6};


// -----------------------------
// Setup
// -----------------------------
void setup() {

  Serial.begin(9600);
  SPI.begin();

  // Initialize RFID readers
  rfid1.PCD_Init();
  rfid2.PCD_Init();
  rfid3.PCD_Init();

  // Configure traffic light pins
  pinMode(R1, OUTPUT); pinMode(Y1, OUTPUT); pinMode(G1, OUTPUT);
  pinMode(R2, OUTPUT); pinMode(Y2, OUTPUT); pinMode(G2, OUTPUT);
  pinMode(R3, OUTPUT); pinMode(Y3, OUTPUT); pinMode(G3, OUTPUT);

  setAllRed();

  Serial.println("🚦 Smart Traffic System Ready");
}


// -----------------------------
// Main Loop
// -----------------------------
void loop() {

  // Priority 1: RFID Detection
  if (checkRFID(rfid1)) {
    Serial.println("✅ Authorized RFID detected on Road 1");
    activateSignal(1);
  }

  else if (checkRFID(rfid2)) {
    Serial.println("✅ Authorized RFID detected on Road 2");
    activateSignal(2);
  }

  else if (checkRFID(rfid3)) {
    Serial.println("✅ Authorized RFID detected on Road 3");
    activateSignal(3);
  }

  // Priority 2: Serial signal from Python vision system
  if (Serial.available() > 0) {

    char incoming = Serial.read();

    if (incoming == '1') {
      Serial.println("📩 Serial trigger → Road 1");
      activateSignal(1);
    }

    else if (incoming == '2') {
      Serial.println("📩 Serial trigger → Road 2");
      activateSignal(2);
    }

    else if (incoming == '3') {
      Serial.println("📩 Serial trigger → Road 3");
      activateSignal(3);
    }

    else {
      Serial.print("⚠️ Unknown input: ");
      Serial.println(incoming);
    }
  }
}


// -----------------------------
// Check RFID Tag
// -----------------------------
bool checkRFID(MFRC522 &rfid) {

  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return false;

  Serial.print("Detected UID: ");

  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i], HEX);
    Serial.print(" ");
  }

  Serial.println();

  // Check if UID matches authorized ambulance tag
  bool match = true;

  for (byte i = 0; i < 4; i++) {
    if (rfid.uid.uidByte[i] != authorizedUID[i]) {
      match = false;
      break;
    }
  }

  rfid.PICC_HaltA();

  return match;
}


// -----------------------------
// Activate Green Signal
// -----------------------------
void activateSignal(int road) {

  setAllRed();

  // Flash yellow before switching
  if (road == 1) flashYellow(Y1);
  else if (road == 2) flashYellow(Y2);
  else if (road == 3) flashYellow(Y3);

  switch (road) {

    case 1:
      digitalWrite(R1, LOW);
      digitalWrite(G1, HIGH);
      break;

    case 2:
      digitalWrite(R2, LOW);
      digitalWrite(G2, HIGH);
      break;

    case 3:
      digitalWrite(R3, LOW);
      digitalWrite(G3, HIGH);
      break;
  }

  delay(5000);   // Allow ambulance to pass
  setAllRed();
}


// -----------------------------
// Set All Signals to Red
// -----------------------------
void setAllRed() {

  digitalWrite(R1, HIGH); digitalWrite(Y1, LOW); digitalWrite(G1, LOW);
  digitalWrite(R2, HIGH); digitalWrite(Y2, LOW); digitalWrite(G2, LOW);
  digitalWrite(R3, HIGH); digitalWrite(Y3, LOW); digitalWrite(G3, LOW);
}


// -----------------------------
// Flash Yellow Warning
// -----------------------------
void flashYellow(int yellowPin) {

  for (int i = 0; i < 3; i++) {

    digitalWrite(yellowPin, HIGH);
    delay(250);

    digitalWrite(yellowPin, LOW);
    delay(250);
  }
}
