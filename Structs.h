#ifndef BLESensorSettings_h
#define BLESensorSettings_h

#include "BLEDevice.h"

// Hardware
struct HardwareRecord
{
    int id;
    String token;
    String uuid; 
};

// Hardware
struct MonitoringRecord
{
    bool hasPresent;
    int temperature; 
};

typedef struct BLEDeviceConnect
{
  BLEClient* pClient;
  BLERemoteCharacteristic* pRemoteCharacteristic;
  BLERemoteService* pRemoteService;
  String uuid = "";
  bool deviceFound = true;
};

#endif
