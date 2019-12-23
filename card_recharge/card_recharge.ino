#include <Keypad.h>

char hexaKeys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[4] = {9, 8, 7, 6}; 
byte colPins[4] = {5, 4, 3, 2}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, 4, 4); 

void setup(){
  Serial.begin(9600);
}

//Read the amount on the user's RFID card:
int get_card_amount() {
  //TODO
  return 40;
}
//Write the new amount onto the user's RFID card:
void write_new_amount(int amount) {
  //TODO
}

//Get an amount to add to card, then user should press A.
//The value will then be added to the card, and the input is reset.

//Have we read the user's card?
boolean card_read=false;
//Amount stored on user's card:
int current_amount=-1;
//Temporary input string:
String amount_string="";
//Amount as an integer:
int recharge_amount=-1;

void loop(){
  if(card_read) {
    //If amount has not already been provided:
    if(recharge_amount==-1) {
      //Get a key input:
      char temp_key=customKeypad.getKey();
      
      //Get amount to be added:
      if(temp_key>='0' && temp_key<='9'){
        //Get amount and print in the same line:
        amount_string+=temp_key;
        Serial.print(temp_key);
      }
      //Accept amount when user presses 'A':
      else if(temp_key=='A') {
        //Convert value to integer:
        recharge_amount=amount_string.toInt();
        //Reset input string:
        amount_string="";
        Serial.println();
      }
    }
    //If amount has been accepted:
    else {
      int new_amount=current_amount+recharge_amount;
      //Write new amount:
      write_new_amount(new_amount);
      Serial.print("Recharge successful. ");
      Serial.print("New Amount: Rs. ");
      Serial.println(new_amount);
      //Reset amount values:
      current_amount=recharge_amount=-1;
      //Set card_read:
      card_read=false;
    }
  }
  else {
    //Wait for user's card:
    //TODO
    //Read amount and print to console:
    current_amount=get_card_amount();
    Serial.print("Amount in card: Rs. ");
    Serial.println(current_amount);
    //Set card_read:
    card_read=true;
    //Print console prompt:
    Serial.print("Enter recharge amount: ");
  }
}
