#include <Time.h>
#include <TimeLib.h>

#include <SPI.h>
#include <MFRC522.h>

#include<GSM.h>
#include <LiquidCrystal.h>

#include "constants.h"


GSM_SMS sms;
GSM gsmAccess;
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal lcd(LCD_RSPIN, LCD_ENPIN, LCD_DATA1, LCD_DATA2, LCD_DATA3, LCD_DATA4);

time_t startTime, currentTime, setupTime;

void setup() {  
  //start serial port
  Serial.begin(9600);
  
  //start lcd and print first message
  lcd.begin(LCD_NCOL, LCD_NROW);
  lcd.print("Initializaing");
  //Serial.println("Initializing");
  
   SPI.begin();       // Init SPI bus
   mfrc522.PCD_Init(); // Init MFRC522
   Serial.println("RFID reading UID");

  startTime = now();
}

void loop() {
  currentTime = (now() - startTime);

  lcd.clear();
  lcd.print(currentTime);

  //checkGSM();

  checkRFID();

  delay(100);
}

void checkGSM(){
  lcd.clear();
  lcd.print("SENDING SMS");
  delay(5000);
  
  sms.beginSMS(PHONENUMBER);
  sms.print(SMSCONTENT);
  sms.endSMS();

  lcd.clear();
  lcd.print("SMS DONE");
  delay(30000);
}


void checkRFID(){
  Serial.println("Checking");
if ( mfrc522.PICC_IsNewCardPresent())
    {
        if ( mfrc522.PICC_ReadCardSerial())
        {
           Serial.print("Tag UID:");
           for (byte i = 0; i < mfrc522.uid.size; i++) {
                  Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                  Serial.print(mfrc522.uid.uidByte[i], HEX);
            }
          
            Serial.println();
            mfrc522.PICC_HaltA();
        }
}
}


/*
void checkHCSR04(){
    double distance = distanceSensor.measureDistanceCm();
    Serial.println(distance);
    delay(100);
}

void checkLCD(){
  lcd.setCursor(0, 1);
  lcd.print(millis()/1000);
  lcd.print(" Segundos");
}

void checkBuzzerAndLED(){
    tone(BUZZER_PIN,BUZZER_TONE);
    delay(1000);
    noTone(BUZZER_PIN);
    delay(1000);
}

void checkSMS(){
  sms.beginSMS(PHONENUMBER);
  sms.print(SMSCONTENT);
  sms.endSMS();
}*/
