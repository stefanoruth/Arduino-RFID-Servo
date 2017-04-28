#include <SPI.h>
#include <Servo.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);;
MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;
byte block = 1;

Servo servo1;
Servo servo2;
uint8_t pushStart = 130;
uint8_t pushEnd = 45;
uint8_t trash1 = 60;
uint8_t trash2 = 100;
uint8_t trash3 = 135;

void setup() {
	Serial.begin(9600);
	while(!Serial);
	SPI.begin();
	mfrc522.PCD_Init();
	servo1.attach(5);  // Attach the servo on pin 9
	servo2.attach(11); // Attach the servo on pin 10
	servo1.write(trash2);
	servo2.write(pushStart);
	Serial.println(F("Ready"));
}

void loop() {
	// Reset card info
	for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

	// Look for new cards
	if (!mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards
	if (!mfrc522.PICC_ReadCardSerial()) {
		return;
	}

	// Todo
	cardAuthenticate();
	String data = cardRead();
	Serial.println(data);
	Serial.println("Ramp");
	moveRamp(data);
	Serial.println("Push");
	pushTag();
	Serial.println("Next");

	Serial.println();
	mfrc522.PICC_HaltA(); // Halt PICC
	mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
}

void moveRamp(String container) {
	if(container == "Papir") {
		servo1.write(trash1);
	} else if (container == "Metal") {
		servo1.write(trash2);
	} else if (container == "Glas") {
		servo1.write(trash3);
	} else {
		Serial.print("Undefined container: ");
		Serial.println(container);
	}
	delay(1000);
}

void pushTag() {
	servo2.write(pushEnd);
	delay(1000);
	servo2.write(pushStart);
	delay(1000);
}

// Authenticate a card
void cardAuthenticate() {
	status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));

	if (status != MFRC522::STATUS_OK) {
		Serial.print(F("Auth failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		return;
	}
}

String cardRead() {
	byte len = 18;
	byte buffer[len];
	status = mfrc522.MIFARE_Read(block, buffer, &len);

	if (status != MFRC522::STATUS_OK) {
		Serial.print(F("Reading failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		return "";
	}

	String text;

	for (uint8_t i = 0; i < 16; i++) {
		if (buffer[i] != ' ') {
			text += (char)buffer[i];
		}
	}

	return text;
}