
#ifndef xlxConfig_h
#define xlxConfig_h

#include "xliCommon.h"

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
  US id                       :2;           // timezone id
  SHORT offset                :2;           // offser in minutes
  UC dst                      :1;           // daylight saving time flag
} Timezone_t;

typedef struct
{
  UC State                    :1;           // Component state
  UC CW                       :1;           // Brightness of cold white
  UC WW                       :1;           // Brightness of warm white
  UC R                        :1;           // Brightness of red
  UC G                        :1;           // Brightness of green
  UC B                        :1;           // Brightness of blue
} Hue_t;

typedef struct
{
  UC version                  :1;           // Data version, other than 0xFF
  US sensorBitmap             :2;           // Sensor enable bitmap
  UC indBrightness            :1;           // Indicator of brightness
  UC typeMainDevice           :1;           // Type of the main lamp
  UC numDevices               :1;           // Number of devices
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
  UC id                       :1;           // ID, 1 based
  UC type                     :1;           // Type of lamp
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
	BOOL isEnabled		: 1;
	DevStatus_t color;	//160 bits
	UC day				: 3;            //values: 1-7
	UC hour				: 5;            //values: 0-23
	UC min				: 6;			//values: 0-59
	UC sec				: 6;			//values: 0-59
	UC indBrightness	: 8;
} ScheduleTable;

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
  
  ScheduleTable schedule_table[MAX_SCT_ENTRY]; // Schedule Table

public:
  ConfigClass();
  void InitConfig();

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
