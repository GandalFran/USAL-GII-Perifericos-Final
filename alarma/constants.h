#ifndef __CONSTATNS_H
#define __CONSTATNS_H

//--------------------------------------

	#define LCD_RSPIN 8
	#define LCD_ENPIN 9
	#define LCD_DATA1 10
	#define LCD_DATA2 11
	#define LCD_DATA3 12
	#define LCD_DATA4 13
	#define LCD_NROW 2
	#define LCD_NCOL 16

//--------------------------------------

	const byte KEYPAD_NROWS=4, KEYPAD_NCOLS=4;
	char KEYPAD_inputs[KEYPAD_NROWS][KEYPAD_NCOLS] = { {'1','2','3','A'}, {'4','5','6','B'}, {'7','8','9','C'}, {'*','0','#','D'} };
	byte KEYPAD_colPins[KEYPAD_NCOLS] = {26,27,28,29}, KEYPAD_rowPins[KEYPAD_NROWS] = {22,23,24,25 };

//--------------------------------------

	#define BUZZER_PIN 52
	#define BUZZER_TONE 1000
	#define BUZZER_PERIOD 1000

//--------------------------------------

	#define ECHOPIN 6
	#define TRIGGERPIN  7
	#define MAXPERTURBATION 100

//--------------------------------------

  #define CHANGEMODE 'A'
  #define ERASECHAR  '#'

  #define PASSWD_SIZE 6
  #define PASSWD "12345"

  #define MAX_ALERT_TIME (60000)
	#define DELAY 300
  #define NOTIFY_DELAY 5000

//--------------------------------------

typedef enum{ ACTIVE=0, INACTIVE=1, ALERT=2} AlarmState;

#endif
