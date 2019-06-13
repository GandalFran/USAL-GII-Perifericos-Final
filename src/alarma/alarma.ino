#include <Time.h>
#include <TimeLib.h>

#include<GSM.h>

#include <SPI.h>
#include <MFRC522.h>

#include <HCSR04.h>

#include <LiquidCrystal.h>

#include "constants.h"

void(* RESET_ARDUINO) (void) = 0;  // declare reset fuction at address 0

AlarmState state;
time_t alertStartTime;

GSM_SMS sms;
GSM gsmAccess;

MFRC522 mfrc522(SS_PIN, RST_PIN);
UltraSonicDistanceSensor distanceSensor(TRIGGERPIN, ECHOPIN);
LiquidCrystal lcd(LCD_RSPIN, LCD_ENPIN, LCD_DATA1, LCD_DATA2, LCD_DATA3, LCD_DATA4);

void setup() {
  //start serial port
  Serial.begin(9600);
  
  //start lcd and print first message
  lcd.begin(LCD_NCOL, LCD_NROW);
  lcd.print("  Initializing");
  Serial.print("  Initializing");

  //start card reader
  SPI.begin();
  mfrc522.PCD_Init(); 
  
  //start gsm
  gsmAccess.begin("");
  
  //define buffer and led pin
  pinMode(LED_PIN,OUTPUT);
  pinMode(BUZZER_PIN,OUTPUT);

  //set the satate of the alarm inactive
  state = INACTIVE;
  printState();
}


void loop() {
  switch(state){
    case ACTIVE:    alarmActiveBehaviour();   break;
    case INACTIVE:  alarmInactiveBehaviour(); break;
    case ALERT:     alarmAlertBehaviour();    break;
  }

  delay(DELAY);
}

void alarmInactiveBehaviour(){
  if (checkCardReader()){
    state = ACTIVE;
    printState();
  }  
}

void alarmActiveBehaviour(){
    //check if its necessary to enter in alert mode  
    if( checkDistance() ){
        //set the alert mode
        state = ALERT;
        printState();
        //start the sound and led
        tone(BUZZER_PIN,BUZZER_TONE);
        digitalWrite(LED_PIN,HIGH);
        //save current time
        alertStartTime = now();
    }

    //chek if alarm deactivation is required
    if ( checkCardReader()){
     lcd.clear(); 
     lcd.print("     ALARM");
     lcd.setCursor(0,1);
     lcd.print("   DEACTIVATED");
     delay(NOTIFY_DELAY);
     //reset arduino
     RESET_ARDUINO(); 
    }
}

void alarmAlertBehaviour(){
  time_t currentTime = now() - alertStartTime;
  long remainingTime= MAX_ALERT_TIME - currentTime;  

  //check if the state 
  if( remainingTime < 0){
     //stop sound and led
     noTone(BUZZER_PIN);
     digitalWrite(LED_PIN,LOW);
     //send a SMS
     sendSMS();
     //notify sms sent
     lcd.clear(); 
     lcd.print("TIME LIMIT REACHED");
     lcd.setCursor(0,1);
     lcd.print("     SMS SENT     ");
     delay(NOTIFY_DELAY);
     //reset arduino
     RESET_ARDUINO();
  }

  //check if deactivation is required
  if( checkCardReader()){
     //stop sound and led
     noTone(BUZZER_PIN);
     digitalWrite(LED_PIN,LOW);
     //print
     lcd.clear(); 
     lcd.print("     ALARM");
     lcd.setCursor(0,1);
     lcd.print("   DEACTIVATED");
     delay(NOTIFY_DELAY);
     //reset arduino
     RESET_ARDUINO();
  }
  
  printState();
}

boolean checkDistance(){
  static double lastDistance = -2;
  double distance = distanceSensor.measureDistanceCm();
  boolean result = ( abs(distance - lastDistance) > MAXPERTURBATION );
  lastDistance = distance;
  return result;
}

boolean checkCardReader(){
  int i;
  boolean isCodeOK = false;
  if ( mfrc522.PICC_IsNewCardPresent() ){
    if( mfrc522.PICC_ReadCardSerial()){
      isCodeOK = true;
      isCodeOK = isCodeOK && (mfrc522.uid.size == CODE_SIZE);
      for(i=0; i<CODE_SIZE && i<mfrc522.uid.size && isCodeOK; i++)
        isCodeOK = isCodeOK && (cardCode[i] == mfrc522.uid.uidByte[i]);
      mfrc522.PICC_HaltA();
    }
  }      
  return isCodeOK;
}

void sendSMS(){
  sms.beginSMS(PHONENUMBER);
  sms.print(SMSCONTENT);
  sms.endSMS();
}

void printState(){
    lcd.clear(); 
    switch(state){
      case ACTIVE:    
          lcd.print("     ALARM");
          lcd.setCursor(0,1);
          lcd.print("   ACTIVATED");
          Serial.print("     ALARM\n");
          Serial.print("   ACTIVATED");
          break;
      case INACTIVE:    
          lcd.print("     ALARM");
          lcd.setCursor(0,1);
          lcd.print("    INACTIVE");
          Serial.print("     ALARM\n");
          Serial.print("   INACTIVE");
          break;
      case ALERT: 
          time_t currentTime = now() - alertStartTime;
          long remainingTime= MAX_ALERT_TIME - currentTime;     
          lcd.print("     ALERT ");  
          lcd.setCursor(0,1); 
          lcd.print("      "); 
          lcd.print(remainingTime);
          lcd.print("s");
          Serial.print("     ALERT\n");
          Serial.print(remainingTime);
          
      break;
  }
}
