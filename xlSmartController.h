//  xlSmartController.h - Xlight SmartController project scale definitions header

#ifndef xlSmartController_h
#define xlSmartController_h

#include "xliCommon.h"
#include "xlxCloudObj.h"
#include "xlxConfig.h"
#include "xlxChain.h"
#include "MyMessage.h"

//------------------------------------------------------------------
// Xlight Command Queue Structures
//------------------------------------------------------------------

//ToDo: Create command queue


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

  String hue_to_string(Hue_t hue);
  bool updateDevStatusRow(MyMessage msg);

public:
  SmartControllerClass();
  void Init();
  void InitRadio();
  void InitNetwork();
  void InitPins();
  void InitSensors();
  void InitCloudObj();

  BOOL Start();
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

  // Process all kinds of commands
  void ProcessCommands();
  void CollectData(UC tick);
  bool ExecuteLightCommand(String mySerialStr);
  
  // Device Control Functions
  int DevSoftSwitch(BOOL sw, UC dev = 0);

  // High speed system timer process
  void FastProcess();

  // Cloud interface implementation
  int CldSetTimeZone(String tzStr);
  int CldPowerSwitch(String swStr);
  int CldJSONCommand(String jsonCmd);
  int CldJSONConfig(String jsonData);

  // Parsing Functions
  bool ParseCmdRow(JsonObject& data);
  String CreateColorPayload(uint8_t ring, uint8_t State, uint8_t CW, uint8_t WW, uint8_t R, uint8_t G, uint8_t B);

  // Cloud Interface Action Types
  bool Change_Rule(RuleRow_t row);
  bool Change_Schedule(ScheduleRow_t row);
  bool Change_Scenario(ScenarioRow_t row);
  bool Action_Rule(ListNode<RuleRow_t> *rulePtr);
  bool Action_Schedule(OP_FLAG parentFlag, UC uid, UC rule_uid);

  bool Change_Sensor();	//ToDo

  //LinkedLists (Working memory tables)
  ChainClass<DevStatusRow_t> DevStatus_table = ChainClass<DevStatusRow_t>(MAX_DEVICE_PER_CONTROLLER);
  ChainClass<ScheduleRow_t> Schedule_table = ChainClass<ScheduleRow_t>(MAX_TABLE_SIZE);
  ChainClass<ScenarioRow_t> Scenario_table = ChainClass<ScenarioRow_t>(MAX_TABLE_SIZE);
  ChainClass<RuleRow_t> Rule_table = ChainClass<RuleRow_t>((int)(MEM_RULES_LEN / sizeof(RuleRow_t))); // 65536/5 = 13107

  //Print LinkedLists (Working memory tables)
  void print_devStatus_table(int row);
  void print_schedule_table(int row);
  void print_scenario_table(int row);
  void print_rule_table(int row);

  // Action Loop & Helper Methods
  void ReadNewRules();
  bool CreateAlarm(ListNode<ScheduleRow_t>* scheduleRow, uint32_t tag = 0);
  bool DestoryAlarm(AlarmId alarmID, UC SCT_uid);

  // UID search functions
  ListNode<ScheduleRow_t> *SearchSchedule(UC uid);
  ListNode<ScenarioRow_t> *SearchScenario(UC uid);
  ListNode<DevStatusRow_t> *SearchDevStatus(UC dest_id); //destination node

  // Utils
  void Array2Hue(JsonArray& data, Hue_t& hue);     // Copy JSON array to Hue structure
};

//------------------------------------------------------------------
// Function & Class Helper
//------------------------------------------------------------------
extern SmartControllerClass theSys;

#endif /* xlSmartController_h */
