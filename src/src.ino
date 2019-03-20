#include <HCSR04.h>
#include "alarmState.h"

#define SERIAL_BAUD 9600
#define DELAY 100

#define PIN_TRIGGER 12
#define PIN_ECHO 13

#define MAX_RANGE 20

//sensor handlers
Alarm alarmStatus;
UltraSonicDistanceSensor sensor(PIN_TRIGGER, PIN_ECHO);

//other vars
double lastDistance;

//Function prototypes
bool isThereAnyIntruder();
void adviseOfIntruder();

void setup() {
  //Serial port initialization for debugging
  Serial.begin(SERIAL_BAUD);

  //variable initialization
  alarmStatus.setStatus(ALARM_ACTIVE);
  lastDistance = sensor.measureDistanceCm();
}

void loop() {
  bool intruderDetected, cardDetected;

  if(alarmStatus.isAlarmActive()){
    
    intruderDetected = isThereAnyIntruder();
    if(intruderDetected){
        adviseOfIntruder();
    }  
    
  }

  cardDetected = checkCardReader();
  if(cardDetected){
    alarmStatus.setStatus( alarmStatus.isAlarmActive() ? ALARM_INACTIVE : ALARM_ACTIVE );
  }

  delay(DELAY);
}

bool isThereAnyIntruder(){
  double currentDistance;
  bool comparationResult;
  
  currentDistance = sensor.measureDistanceCm();
  comparationResult = (currentDistance-lastDistance) > MAX_RANGE;
  lastDistance = currentDistance;

  Serial.println(currentDistance);
  
  return comparationResult;
}


void adviseOfIntruder(){
  
}
