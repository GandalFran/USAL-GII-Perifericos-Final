#include<GSM.h>
#include <SPI.h>
#include <HCSR04.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#include "constants.h"


void(* RESET_ARDUINO) (void) = 0;  // declare reset fuction at address 0

AlarmState state;
unsigned long alertStartTime;

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
  lcd.print("Initializaing");
  Serial.println("Initializing");

  //start card reader
  SPI.begin();
  mfrc522.PCD_Init(); 
  //start gsm
 //gsmAccess.begin(" ");
  //define buffer and led pin
  pinMode(LED_PIN,OUTPUT);
  pinMode(BUZZER_PIN,OUTPUT);

  //set the satate of the alarm inactive
  state = INACTIVE;

  
  Serial.println("Initializing done");
}


void loop() {
  printState();
  
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
  }  
}

void alarmActiveBehaviour(){
    //check if its necessary to enter in alert mode  
    if( checkDistance() ){
        //set the alert mode
        state = ALERT;
        //start the sound and led
        tone(BUZZER_PIN,BUZZER_TONE);
        digitalWrite(LED_PIN,HIGH);
        //save current time
        alertStartTime = millis();
    }

    //chek if alarm deactivation is required
    if ( checkCardReader()){
      //set inactive mode  
    }
}

void alarmAlertBehaviour(){
  long remainingTime = MAX_ALERT_TIME - millis() - alertStartTime;  

  //check if the state 
  if( remainingTime < 0){
     //stop sound and led
     noTone(BUZZER_PIN);
     digitalWrite(LED_PIN,LOW);
     //send a SMS
     sendSMS();
     //notify sms sent
     lcd.clear(); 
     lcd.print("TIME LIMIT REACHED\nSMS SENT");
     delay(NOTIFY_DELAY);
     //reset arduino
     RESET_ARDUINO();
  }

  //check if deactivation is required
  if( checkCardReader()){
     lcd.clear(); 
     lcd.print("ALARM DEACTIVATED");
     delay(NOTIFY_DELAY);
     //reset arduino
     RESET_ARDUINO();
  }
  
}

boolean checkDistance(){
  static double lastDistance = 0;
  double distance = distanceSensor.measureDistanceCm();
  return ( abs(distance - lastDistance) > MAXPERTURBATION );
}

boolean checkCardReader(){
  int i;
  boolean isCodeOK = true;
  if ( mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial() ){
      isCodeOK = isCodeOK && (mfrc522.uid.size == CODE_SIZE);
      for(i=0; i<CODE_SIZE && i<mfrc522.uid.size && isCodeOK; i++)
        isCodeOK = isCodeOK && (cardCode[i] == mfrc522.uid.uidByte[i]);
  } 
  mfrc522.PICC_HaltA();     
  return isCodeOK;
}

void sendSMS(){
  sms.beginSMS(PHONENUMBER);
  sms.print("Hola");
  sms.endSMS();
}

void printState(){
    lcd.clear(); 
    switch(state){
      case ACTIVE:    
          lcd.print("ALARM ACTIVE");     
          lcd.setCursor(0,1);
          lcd.print("Use card to deactivate"); 
          break;
      case INACTIVE:    
          lcd.print("ALARM INACTIVE");   
          lcd.setCursor(0,1);
          lcd.print("Use card to activate"); 
          break;
      case ALERT: 
          long remainingTime = MAX_ALERT_TIME - millis() - alertStartTime;    
          lcd.print("ALERT");    
          lcd.print(remainingTime/1000);lcd.print("s");  
          lcd.setCursor(0,1);
      break;
  }
}

#ifdef __DEBUG

void checkAll(){
  static long alertStartTime = millis();
  long currentTime = (millis() - alertStartTime)/1000;  

  checkLCD();

  if(currnetTime < 3){
    checkHCSR04();  
  }else if(currentTime< 5){
    checkCardReader();  
  }else if(currentTime< 8){
    checkBuzzerAndLED();  
  }else if(currentTime<9){
    checkSMS();  
  }
  
}

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
}
#endif
