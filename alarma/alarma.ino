#include <HCSR04.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#include "constants.h"

AlarmState state;
unsigned long alertStartTime;
int currentPasswordIndex;
char currentPassword[PASSWD_SIZE];

UltraSonicDistanceSensor distanceSensor(TRIGGERPIN, ECHOPIN);
LiquidCrystal lcd(LCD_RSPIN, LCD_ENPIN, LCD_DATA1, LCD_DATA2, LCD_DATA3, LCD_DATA4);
Keypad keypad = Keypad( makeKeymap(KEYPAD_inputs),KEYPAD_rowPins,KEYPAD_colPins,KEYPAD_NROWS,KEYPAD_NCOLS);

void setup() {
  //start serial port
  Serial.begin(9600);
  
  //start lcd
  lcd.begin(LCD_NCOL, LCD_NROW);

  //set the satate of the alarm inactive
  state = INACTIVE;

  //set the current password index to zero
  currentPasswordIndex = 0;

  //print lcd first message
  lcd.clear(); lcd.print("ALARM INACTIVE"); lcd.setCursor(0,1); lcd.print("Pulse "); lcd.print(CHANGEMODE); lcd.print(" to start");
}


void loop() {
  switch(state){
    case ACTIVE:    alarmActiveBehaviour();   break;
    case INACTIVE:  alarmInactiveBehaviour(); break;
    case ALERT:     alarmAlertBehaviour();    break;
  }
}

void alarmInactiveBehaviour(){
  char input = keypad.getKey();
  Serial.println(input);
  if (input == CHANGEMODE){
    lcd.clear(); lcd.print("ALARM ACTIVE"); lcd.setCursor(0,1); lcd.print("Pulse "); lcd.print(CHANGEMODE); lcd.print(" to stop"); 
    state = ACTIVE;
  }
  delay(DELAY);  
}

void alarmActiveBehaviour(){
    static double lastDistance = 0;
    double distance;

    //check distance
    distance = distanceSensor.measureDistanceCm();
    if( abs(distance - lastDistance) > MAXPERTURBATION ){
        //set the alert mode
        state = ALERT;
        //start the sound
        tone(BUZZER_PIN,BUZZER_TONE);
        //reset the last passwd
        currentPasswordIndex = 0;
        memset(currentPassword,0,PASSWD_SIZE);
        //save current time
        alertStartTime = millis();
    }

    //chek if deactivation is required
    char input = keypad.getKey();
    if(currentPasswordIndex > 0 || input == CHANGEMODE){
        readPasswordCharacter(input);
    }
}

void alarmAlertBehaviour(){
   long remainingTime = MAX_ALERT_TIME - millis() - alertStartTime;
   
   lcd.clear();lcd.print("ALERT ");lcd.print(remainingTime/1000);lcd.print("s");

  if( remainingTime < 0){
    //stop sound
     noTone(BUZZER_PIN);
     
    //send a SMS

    //notify sms sent
     lcd.clear(); lcd.print("TIME LIMIT REACHED\nSMS SENT");
     delay(NOTIFY_DELAY);
     
     //print new mode
     lcd.clear(); lcd.print("ALARM INACTIVE"); lcd.setCursor(0,1); lcd.print("Pulse "); lcd.print(CHANGEMODE); lcd.print(" to start"); 

     //set inactive mode 
     state = INACTIVE;
  }else{
      readPasswordCharacter(keypad.getKey());
  }
  
  delay(DELAY);
}

void readPasswordCharacter(char input){
      if(input == ERASECHAR && currentPasswordIndex>0){
          currentPassword[currentPasswordIndex--] = '\0';
      }else if(input != 0){
        currentPassword[currentPasswordIndex++] = input;
        if(!strcmp(PASSWD,currentPassword)){
              //notify correct password
              lcd.clear(); lcd.print("CORRECT PASSWORD");
              delay(NOTIFY_DELAY);
              
              //print new mode
              lcd.clear(); lcd.print("ALARM INACTIVE"); lcd.setCursor(0,1); lcd.print("Pulse "); lcd.print(CHANGEMODE); lcd.print(" to start"); 
              
              //set the inactive mode
              state = INACTIVE;  
        }
      }
      
      //clear second line of lcd
      lcd.setCursor(0,1); lcd.print("        ");
      lcd.setCursor(0,1); lcd.print(currentPassword);
}

#ifdef __DEBUG
void checkHCSR04(){
    double distance = distanceSensor.measureDistanceCm();
    Serial.println(distance);
    lcd.setCursor(0, 0);
    lcd.println(distance);
    delay(500);
}

void checkLCD(){
  lcd.setCursor(0, 1);
  lcd.print(millis()/1000);
  lcd.print(" Segundos");
}

void checkKeyPad(){
  char input = keypad.getKey();
  if (input != 0){
    Serial.println(input);
    lcd.print(input);
  }
}

void checkBuzzer(){
    tone(BUZZER_PIN,BUZZER_TONE);
    delay(1000);
    noTone(BUZZER_PIN);
    delay(1000);
}

void checkSMS(){
  return;
}
#endif
