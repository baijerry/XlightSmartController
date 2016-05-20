//  xlSmartController.h - Xlight SmartController project scale definitions header

#ifndef xlSmartController_h
#define xlSmartController_h

#include "xliCommon.h"
#include "xlxCloudObj.h"
#include "TimeAlarms.h"

class SmartControllerClass;           // forward reference

//------------------------------------------------------------------
// Smart Controller Class
//------------------------------------------------------------------
class SmartControllerClass : public CloudObjClass
{
private:
  BOOL m_isRF;
  BOOL m_isBLE;
  BOOL m_isLAN;
  BOOL m_isWAN;

public:
  SmartControllerClass();
  void Init();
  void InitRadio();
  void InitNetwork();
  void InitPins();
  void InitSensors();
  void InitCloudObj();

  BOOL Start();
  String GetSysID();
  UC GetStatus();
  void SetStatus(UC st);

  BOOL CheckRF();
  BOOL CheckNetwork();
  BOOL CheckBLE();
  BOOL SelfCheck(UL ms);
  BOOL IsRFGood();
  BOOL IsBLEGood();
  BOOL IsLANGood();
  BOOL IsWANGood();

  void CollectData(UC tick);

  // Device Control Functions
  int DevSoftSwitch(BOOL sw, UC dev = 0);

  // High speed system timer process
  void FastProcess();

  // Cloud interface implementation
  int CldSetTimeZone(String tzStr);
  int CldPowerSwitch(String swStr);
  int CldJSONCommand(String jsonData);

  // Alarms and Timers
  void UpdateAlarms(int action, int index, BOOL isRepeat, UC day, UC hour, UC min, UC sec, AlarmId alarm_id); //creates or deletes alarms
  void AlarmTimerTriggered(int SCTindex);
};

//------------------------------------------------------------------
// Function & Class Helper
//------------------------------------------------------------------
extern SmartControllerClass theSys;

#endif /* xlSmartController_h */
