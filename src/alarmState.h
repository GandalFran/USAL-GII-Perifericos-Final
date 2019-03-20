#ifndef __ALARMSTATE_H
#define __ALARMSTATE_H

  typedef enum{ALARM_ACTIVE,ALARM_INACTIVE}AlarmStatus;
  
  class Alarm { 
    private:  
      AlarmStatus alarmStatus; 
    public:
      Alarm();
      void setStatus(AlarmStatus newStatus);
      AlarmStatus getStatus();
      bool isAlarmActive(); 
  };

#endif
