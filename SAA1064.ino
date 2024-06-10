#include <Wire.h> //include Wire.h library
byte x = 0x00;

//Four different slave addresses can be chosen by connecting ADR either to VEE, 3/8 VCC, 5/8 VCC or VCC. This results in
//the corresponding valid addresses HEX 70, 72, 74 and 76 for writing and 71, 73, 75 and 77 for reading. All other
//addresses cannot be acknowledged by the circuit where we using a SAA1064 driver.
/*
Control bits presents at 2º byte
C0 = 0 static mode, i.e. continuous display of digits 1 and 2
C0 = 1 dynamic mode, i.e. alternating display of digit 1 + 3 and 2 + 4
C1 = 0/1 digits 1 + 3 are blanked/not blanked
C2 = 0/1 digits 2 + 4 are blanked/not blanked
C3 = 1 all segment outputs are switched-on for segment test(1)
C4 = 1 adds 3 mA to segment output current
C5 = 1 adds 6 mA to segment output current
C6 = 1 adds 12 mA to segment output current
*/

char str[7] = {0x7F, 0x07, 0x0F, 0x0F, 0x0F, 0x0F};
char msgHello[] =  {0b00000000, 0b01110110, 0b01111001, 0b00111000, 0b00111000, 0b00111111};
char msgNumbers[] =  {0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110,  
                      0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111};

void setup() {
  //Wire.setClock(100000); //Frquency used at communitation
  Wire.begin(); // Initialize I2C (Master Mode: address is optional)
  Serial.begin(115200);

   pinMode(18, OUTPUT); //SDA pin A4
   pinMode(19, OUTPUT); //SCL pin A5
   pinMode(8, OUTPUT);  //This is to control LED (Go throug a resistor of 10K to base of transistor presente at panel)
   pinMode(9, OUTPUT);  //This is to control LED (Go throug a resistor of 10K to base of transistor presente at panel)
   pinMode(10, OUTPUT); //This is to control LED (Go throug a resistor of 10K to base of transistor presente at panel)
    //
}
void LEDs(){
   digitalWrite(8, HIGH);
   delay(600); 
   digitalWrite(8, LOW);
    //
   digitalWrite(9, HIGH);
   delay(600); 
   digitalWrite(9, LOW);
    //
   digitalWrite(10, HIGH);
   delay(600); 
   digitalWrite(10, LOW);
}
void testDSP(){
  Wire.beginTransmission(0x38); // transmit to device #?
  // 00000 SC,SB,SA
  Wire.write(0x00);  Wire.write(0b01000111); //The first byte set as Auto-Increment and allow the second byte as control status
  //        D17~D10;           D27~D20            D37~D30            D47~D40
  //1ºdigit:   hgfedcba    2ºdigit:   hgfedcba 3ºdigit:   hgfedcba  4ºdigit:   hgfedcba
  Wire.write(0b11111111);  Wire.write(0xFF);  Wire.write(0xFF);  Wire.write(0xFF);
  Wire.endTransmission();    // stop transmitting
  delay(500);
Serial.println("line!");
}
void wrMsg(){
  for (uint8_t i = 0x00; i < 10; i++){
  Wire.beginTransmission(0x38); // transmit to device #?
  // 00000 SC,SB,SA
  Wire.write(0x00);  Wire.write(0b01000111); //The first byte set as Auto-Increment and allow the second byte as control status
  //        D17~D10;           D27~D20            D37~D30            D47~D40
  //  1ºdigit:   hgfedcba    2ºdigit:   hgfedcba 3ºdigit:   hgfedcba  4ºdigit:   hgfedcba
  //Wire.write(msgHello[i]);  Wire.write(msgHello[i]);  Wire.write(msgHello[i]);  Wire.write(msgHello[i+1]);
  
   switch (i) {
    case 0: Wire.write(msgHello[0]);  Wire.write(msgHello[0]);  Wire.write(msgHello[0]);  Wire.write(msgHello[1]);  break;
    case 1: Wire.write(msgHello[0]);  Wire.write(msgHello[0]);  Wire.write(msgHello[1]);  Wire.write(msgHello[2]); break;
    case 2: Wire.write(msgHello[0]);  Wire.write(msgHello[1]);  Wire.write(msgHello[2]);  Wire.write(msgHello[3]); break;
    case 3: Wire.write(msgHello[1]);  Wire.write(msgHello[2]);  Wire.write(msgHello[3]);  Wire.write(msgHello[4]); break;
    case 4: Wire.write(msgHello[2]);  Wire.write(msgHello[3]);  Wire.write(msgHello[4]);  Wire.write(msgHello[5]);  break;
    }
  Wire.endTransmission();    // stop transmitting
  //Serial.println(i, DEC); //Only to debug, comment it!
  delay(500);
  }
}
void wrNumbers(){
  for (uint8_t d = 0x00; d < 4; d++){  //Count from 0 until 3, this will do a trip around 4 digits.
  for (uint8_t i = 0x00; i <= 0x0A; i++){  //Count from 0 until 9, (0x00~0x0B) 
  Wire.beginTransmission(0x38); // transmit to device #?, our SAA1064
  // 00000 SC,SB,SA
  Wire.write(0x00);  Wire.write(0b01000111); //The first byte set as Auto-Increment and allow the second byte as control status
  //        D17~D10;           D27~D20            D37~D30            D47~D40
  //  1ºdigit:   hgfedcba    2ºdigit:   hgfedcba 3ºdigit:   hgfedcba  4ºdigit:   hgfedcba
  //Wire.write(msgHello[i]);  Wire.write(msgHello[i]);  Wire.write(msgHello[i]);  Wire.write(msgHello[i+1]);
  
    switch (d) {
    case 0: Wire.write(msgNumbers[i]);  Wire.write(msgNumbers[0]);  Wire.write(msgNumbers[0]);  Wire.write(msgNumbers[0]);  break;
    case 1: Wire.write(msgNumbers[0]);  Wire.write(msgNumbers[i]);  Wire.write(msgNumbers[0]);  Wire.write(msgNumbers[0]); break;
    case 2: Wire.write(msgNumbers[0]);  Wire.write(msgNumbers[0]);  Wire.write(msgNumbers[i]);  Wire.write(msgNumbers[0]); break;
    case 3: Wire.write(msgNumbers[0]);  Wire.write(msgNumbers[0]);  Wire.write(msgNumbers[0]);  Wire.write(msgNumbers[i]); break;
    }
 
  //Wire.write(str, 6);
  // Wire.write("x is ");        // sends five bytes
  // Wire.write(x);              // sends one byte
  Wire.endTransmission();    // stop transmitting
  Serial.println(i, DEC);
  delay(500);
  }
  }
}

void ctrlBright(uint8_t miliamp){
  Wire.beginTransmission(0x38); // transmit to device #?
  // 00000 SC,SB,SA
  miliamp = (0b00000111 | miliamp);
  Wire.write(0x00);  Wire.write(miliamp); //The first byte set as Auto-Increment and allow the second byte as control status
  //        D17~D10;           D27~D20            D37~D30            D47~D40
  //  1ºdigit:   hgfedcba    2ºdigit:   hgfedcba 3ºdigit:   hgfedcba  4ºdigit:   hgfedcba
  //Wire.write(msgHello[i]);  Wire.write(msgHello[i]);  Wire.write(msgHello[i]);  Wire.write(msgHello[i+1]);
  Wire.write(msgNumbers[8]);  Wire.write(msgNumbers[8]);  Wire.write(msgNumbers[8]);  Wire.write(msgNumbers[8]);  
  Wire.endTransmission();    // stop transmitting
  delay(500);
}
void loop() {
  uint8_t current=0b00010000; //The bits to control the current are the bit: 4, 5 and 6.
  LEDs();
  testDSP();
//The next for is to control the bright, it is available with 3 levels.
    for(uint8_t mili = 0; mili < 3; mili++){
    current = (0b00010000 << mili);
    ctrlBright(current);
    Serial.println(mili, HEX);
    delay(800);
    }

  wrMsg();
  delay(500);
  wrNumbers();
  delay(500);

}
// void requestEvent() {
//   Wire.write("hello "); // respond with message of 6 bytes
//   // as expected by master
// }
