#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
MFRC522::MIFARE_Key key;
MFRC522::StatusCode status;
byte block = 1;

void setup() {
	Serial.begin(9600);		// Initialize serial communications with the PC
	while(!Serial);		// Wait for serial to be ready
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522 card
	randomSeed(analogRead(0));
	//mfrc522.PCD_DumpVersionToSerial();
	Serial.println(F("Ready"));
}

void loop() {
	// Reset card info
	for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

	// Look for new cards
	if ( ! mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {
		return;
	}

	cardDetected();
	authenticateCard();
	writeCard(cardTypeGenerator());
	authenticateCard();
	readCard();

	//delay(1000);
	Serial.println();
	mfrc522.PICC_HaltA(); // Halt PICC
	mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
}

// Authenticate a card
void authenticateCard() {
	status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));

	if (status != MFRC522::STATUS_OK) {
		Serial.print(F("Auth failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		return;
	}
}

// Print the cards UID
void cardDetected() {
	Serial.print("Tag Detected: ");
	for (byte i = 0; i < mfrc522.uid.size; i++) {
		Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
		Serial.print(mfrc522.uid.uidByte[i], HEX);
	}
	Serial.println();
}

// Write new data to a card.
void writeCard(String text) {
	byte buffer[34];
	text += " ";
	text.getBytes(buffer, text.length());
	for (byte i =  text.length(); i < sizeof(buffer) - text.length(); i++) buffer[i] = ' ';

	status = mfrc522.MIFARE_Write(block, buffer, 16);

	if (status != MFRC522::STATUS_OK) {
		Serial.print(F("Write failed: #"));
		Serial.print(status);
		Serial.print(" ");
		Serial.println(mfrc522.GetStatusCodeName(status));
		return;
	}
	
	Serial.print(F("Write success: "));
	Serial.println(text);
}

// Read data from a card.
void readCard() {
	byte len = 18;
	byte buffer[len];
	status = mfrc522.MIFARE_Read(block, buffer, &len);
	if (status != MFRC522::STATUS_OK) {
		Serial.print(F("Reading failed: "));
		Serial.println(mfrc522.GetStatusCodeName(status));
		return;
	}

	Serial.print("Read: ");

	//PRINT FIRST NAME
	for (uint8_t i = 0; i < 16; i++) {
		if (buffer[i] != ' ') {
			Serial.write(buffer[i]);
		}
	}
	Serial.println();
}

// Genrate a random type for a tags
String cardTypeGenerator() {
	switch(random(1,4)) {
		case 1:
			return "Papir";
		case 2:
			return "Metal";
		case 3:
			return "Glas";
	}
}