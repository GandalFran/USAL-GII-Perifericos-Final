#ifndef __CONSTATNS_H
#define __CONSTATNS_H

//--------------------------------------

  #define LCD_RSPIN 33
  #define LCD_ENPIN 31 
  #define LCD_DATA1 29
  #define LCD_DATA2 27
  #define LCD_DATA3 25
  #define LCD_DATA4 23
  #define LCD_NROW 2
  #define LCD_NCOL 16

//--------------------------------------

  #define SS_PIN 53
  #define RST_PIN 49
  #define CODE_SIZE 4
  byte cardCode[CODE_SIZE] =  {0xC5, 0x6B, 0xC2, 0x59};

//--------------------------------------

  #define LED_PIN 35
  #define BUZZER_PIN 37 
  #define BUZZER_TONE 1000
  #define BUZZER_PERIOD 1000

//--------------------------------------

  #define ECHOPIN 39
  #define TRIGGERPIN 41  
  #define MAXPERTURBATION 100

//--------------------------------------

  #define MAX_ALERT_TIME 60
  #define DELAY 100
  #define NOTIFY_DELAY 5000

//--------------------------------------

  #define PHONENUMBER "+34688912649"
  #define SMSCONTENT "Hablame por whatsap si te llega esto"

//--------------------------------------

typedef enum{ ACTIVE=0, INACTIVE=1, ALERT=2} AlarmState;

#endif
