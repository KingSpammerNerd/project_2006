//Library for RC522 RFID Reader
#include <MFRC522.h>
#include <SPI.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

#define FLOAT_SIZE      4

//Key for card operations:
MFRC522::MIFARE_Key key;

//Global variables:
long odometer=0;
float amount=0;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
}

//Get balance value on card
float readBalanceFromCard(MFRC522::MIFARE_Key key) {
  //Cancel if card cannot be read:
  /*if(!mfrc522.PICC_ReadCardSerial()) {
    Serial.println("Card not found \n");
    return -1;
  }*/
  //Authenticate before reading card:
  MFRC522::StatusCode status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid));
  //Cancel if card can't be read:
  if(status != MFRC522::STATUS_OK) {
    Serial.println("Authentication failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return -1;
  }
  //Buffer to hold balance:
  byte balanceBuffer[4];
  //Read balance from card (block 0):
  status=mfrc522.MIFARE_Read(0, balanceBuffer, FLOAT_SIZE);
  if(status!=MFRC522::STATUS_OK) {
    Serial.println(mfrc522.GetStatusCodeName(status));
    return -2;
  }
  //Convert to float and return:
  float amount=String((char*)balanceBuffer).toFloat();
  return amount;
}

//Write new balance value to card
float writeBalanceToCard(MFRC522::MIFARE_Key key, float amount) {
  //Cancel if card cannot be read:
  if(!mfrc522.PICC_ReadCardSerial()) return -1;
  //Authenticate before reading card:
  MFRC522::StatusCode status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 4, &key, &(mfrc522.uid));
  //Cancel if card can't be read:
  if(status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return -1;
  }
  //Convert balance to bytes:
  byte balanceBuffer[4];
  String amtString(amount);
  amtString.toCharArray(balanceBuffer, 4);
  //Write:
  status=mfrc522.MIFARE_Write(0, balanceBuffer, FLOAT_SIZE);
  if(status!=MFRC522::STATUS_OK) Serial.println(mfrc522.GetStatusCodeName(status));
}

//Read distance value to card
float readDistanceFromCard(MFRC522::MIFARE_Key key) {
  //TODO
}

//Write odometer value to card
void writeDistanceToCard(MFRC522::MIFARE_Key key, long odometer) {
  //TODO
}

//Check if a card is present:
bool isCardPresent() {
  return mfrc522.PICC_IsNewCardPresent();
}

//Handle a carad swipe:
void handleCardSwipe() {
  //Set the card's key:
  MFRC522::MIFARE_Key key;
  for (byte i=0; i<6; i++) key.keyByte[i]=0xFF;
  //Read balance from card:
  float cardBalance=readBalanceFromCard(key);
  //Read distance from card:
  float distance=readDistanceFromCard(key);
  //If balance<=0, stop:
  if(cardBalance<=0) {
    Serial.println("Card has no balance!");
    return;
  }
  //If distance==-1, write odometer to card
  //Else, 
}

void loop() {
  //Increment Distance:
  //TODO
  //Check for card swipe:
  if(isCardPresent()) //handleCardSwipe();
  {
    //Set the card's key:
    MFRC522::MIFARE_Key key;
    for (byte i=0; i<6; i++) key.keyByte[i]=0xFF;
    Serial.println("Writing...");
    writeBalanceToCard(key, 5);
    Serial.println("Reading...");
    Serial.println(readBalanceFromCard(key));
  }
}
