#ifndef __CONSTATNS_H
#define __CONSTATNS_H

//--------------------------------------

	#define LCD_RSPIN 25
	#define LCD_ENPIN 23 
	#define LCD_DATA1 33
	#define LCD_DATA2 31
	#define LCD_DATA3 29
	#define LCD_DATA4 27
	#define LCD_NROW 2
	#define LCD_NCOL 16

//--------------------------------------

  #define RST_PIN 47     //Pin 9 para el reset del RC522
  #define SS_PIN 37    //Pin 10 para el SS (SDA) del RC522
  #define CODE_SIZE 4
  byte cardCode[CODE_SIZE] =  {0xC1, 0x2F, 0xD6, 0x0E};

//--------------------------------------

  #define LED_PIN 49
	#define BUZZER_PIN 51 
	#define BUZZER_TONE 1000
	#define BUZZER_PERIOD 1000

//--------------------------------------

	#define ECHOPIN 53
	#define TRIGGERPIN 52  
	#define MAXPERTURBATION 100

//--------------------------------------

  #define MAX_ALERT_TIME 60000
	#define DELAY 300
  #define NOTIFY_DELAY 5000

//--------------------------------------

  #define PHONENUMBER "+34636627009"
  #define SMSCONTENT "Esto es un mensaje de prueba"

//--------------------------------------

typedef enum{ ACTIVE=0, INACTIVE=1, ALERT=2} AlarmState;

#endif
