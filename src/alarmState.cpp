#include "alarmState.h"
  
  Alarm::Alarm() : alarmStatus(ALARM_ACTIVE) { }
  
  void  Alarm::setStatus(AlarmStatus newStatus){
    alarmStatus = newStatus;
  }
    
  bool Alarm::isAlarmActive(){
    return (alarmStatus == ALARM_ACTIVE); 
  }

