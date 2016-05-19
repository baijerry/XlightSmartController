
#ifndef xlxConfig_h
#define xlxConfig_h

#include "xliCommon.h"
#include "xliMemoryMap.h"
#include "TimeAlarms.h"

// Change it only if Config_t structure is updated
#define VERSION_CONFIG_DATA   1

// Xlight Application Identification
#define XLA_ORGANIZATION          "xlight.ca"               // Default value. Read from EEPROM
#define XLA_PRODUCT_NAME          "XController"             // Default value. Read from EEPROM
#define XLA_AUTHORIZATION         "use-token-auth"
#define XLA_TOKEN                 "your-access-token"       // Can update online

//------------------------------------------------------------------
// Xlight Configuration Data Structures
//------------------------------------------------------------------
typedef struct
{
<<<<<<< HEAD
  US id                       :16;           // timezone id
  SHORT offset                :16;           // offser in minutes
  UC dst                      :8;           // daylight saving time flag
=======
  US id;                                    // timezone id
  SHORT offset;                             // offser in minutes
  UC dst                      :1;           // daylight saving time flag
>>>>>>> upstream/master
} Timezone_t;

typedef struct
{
<<<<<<< HEAD
  UC State                    :8;           // Component state
=======
  UC State                    :4;           // Component state
>>>>>>> upstream/master
  UC CW                       :8;           // Brightness of cold white
  UC WW                       :8;           // Brightness of warm white
  UC R                        :8;           // Brightness of red
  UC G                        :8;           // Brightness of green
  UC B                        :8;           // Brightness of blue
} Hue_t;

typedef struct
{
<<<<<<< HEAD
  UC version                  :8;           // Data version, other than 0xFF
  US sensorBitmap             :16;           // Sensor enable bitmap
  UC indBrightness            :8;           // Indicator of brightness
=======
  UC version                  :4;           // Data version, other than 0xFF
  US sensorBitmap             :16;          // Sensor enable bitmap
  UC indBrightness            :4;           // Indicator of brightness
>>>>>>> upstream/master
  UC typeMainDevice           :8;           // Type of the main lamp
  UC numDevices               :8;           // Number of devices
  Timezone_t timeZone;                      // Time zone
  char Organization[24];                    // Organization name
  char ProductName[24];                     // Product name
  char Token[64];                           // Token
} Config_t;

//------------------------------------------------------------------
// Xlight Device Status Structures
//------------------------------------------------------------------
typedef struct
{
  UC id;                                    // ID, 1 based
  UC type;                                  // Type of lamp
  Hue_t ring1;
  Hue_t ring2;
  Hue_t ring3;
} DevStatus_t;

//------------------------------------------------------------------
// Xlight Schedule Data Structures
//------------------------------------------------------------------

enum SCT_STATE {
	SCTempty = 0, //row empty
	SCTnew,		  //new row
	SCTdelete,    //delete row
	SCTactive	  //active row
};

typedef struct //Schedule Table: 25 bytes due to padding
{
	SCT_STATE state		: 2;            //values: 0-3
	BOOL isRepeat		: 1;
	//BOOL isEnabled		: 1;
	UC deviceID			: 5;			//values: 1-16
	UC actionID			: 4;			//values: 0-15
	Hue_t ring1;		//48 bits
	Hue_t ring2;		//48 bits
	Hue_t ring3;		//48 bits
	UC day				: 3;            //values: 1-7
	UC hour				: 5;            //values: 0-23
	UC min				: 6;			//values: 0-59
	UC sec				: 6;			//values: 0-59
	UC indBrightness	: 8;
	AlarmId alarm_id	: 8;
} ScheduleTable;

// Maximun number of entries in Schedule Table (SCT max length: 256 bytes)
#define MAX_SCT_ENTRY			  (int)(256/sizeof(ScheduleTable))

//------------------------------------------------------------------
// Xlight Configuration Class
//------------------------------------------------------------------
class ConfigClass
{
private:
  BOOL m_isLoaded;
  BOOL m_isChanged;         // Config Change Flag
  BOOL m_isDSTChanged;      // Device Status Table Change Flag
  BOOL m_isSCTChanged;      // Schedule Table Change Flag
  Config_t m_config;

public:
  ConfigClass();
  void InitConfig();

  ScheduleTable schedule_table[MAX_SCT_ENTRY]; // Schedule Table

  BOOL LoadConfig();
  BOOL SaveConfig();
  BOOL IsConfigLoaded();
  BOOL IsConfigChanged();
  BOOL IsDSTChanged();
  BOOL IsSCTChanged();
  BOOL UpdateSCT(ScheduleTable row);

  UC GetVersion();
  BOOL SetVersion(UC ver);

  US GetTimeZoneID();
  BOOL SetTimeZoneID(US tz);

  UC GetDaylightSaving();
  BOOL SetDaylightSaving(UC flag);

  SHORT GetTimeZoneOffset();
  SHORT GetTimeZoneDSTOffset();
  BOOL SetTimeZoneOffset(SHORT offset);
  String GetTimeZoneJSON();

  String GetOrganization();
  void SetOrganization(const char *strName);

  String GetProductName();
  void SetProductName(const char *strName);

  String GetToken();
  void SetToken(const char *strName);

  BOOL IsSensorEnabled(sensors_t sr);
  void SetSensorEnabled(sensors_t sr, BOOL sw = true);

  UC GetBrightIndicator();
  BOOL SetBrightIndicator(UC level);

  UC GetMainDeviceType();
  BOOL SetMainDeviceType(UC type);

  UC GetNumDevices();
  BOOL SetNumDevices(UC num);
};

//------------------------------------------------------------------
// Function & Class Helper
//------------------------------------------------------------------
extern ConfigClass theConfig;

#endif /* xlxConfig_h */
