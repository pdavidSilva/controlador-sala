#ifndef BLEServerService_h
#define BLEServerService_h

#include <unordered_map>
#include "Hardware.h"
#include <Arduino.h>
#include "BLEScan.h"
#include <BLEDevice.h>
#include "Structs.h"


#define TYPE_SENSOR  0
#define TYPE_ACTUATOR  1
#define TIME_CONNECTION  10000 

static BLEUUID SERVICE_UUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
static BLEUUID CHARACTERISTIC_UUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");

using namespace std;


class BLEServerService 
{
  private:
   
    int __countTypeSensor;
    int __countTypeActuator;
    vector<String> __sensors;
    vector<String> __actuators;
    bool __receivedRequest;
    BLEScan* __pBLEScan;
    vector<BLEAdvertisedDevice*> __filteredDevices;
    unordered_map<string, Hardware> __devicesMapped;
    
  public:
    BLEServerService();
    
    static void notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify); 
    BLEDeviceConnect* connectToDevice(BLEAdvertisedDevice* myDevice, bool validateConnection); 
    void initBLE(); 
    void scanDevices(); 
    void populateMap(); 
    bool isSensor(String uuid);
    bool isAtuador(String uuid);
    bool connectMyDisp(BLEAdvertisedDevice* device); 
    void sendMessageToActuator(String data, String device);
    int getCountDispsTypeSensor();
    void timer(); 

    // getters and setters
    vector<String> getSensors();
    vector<String> getActuators();
    void addSensor(String uuid);
    void addActuator(String uuid);
    void setReceivedRequest(bool receivedRequest);
    bool getReceivedRequest();
    
    void setCountTypeSensor(int count);
    void setCounttypeActuator(int count);
    int getCountTypeSensor();
    int getCounttypeActuator();

    // metods task
    void continuousConnectionTask();
    static void startTaskImpl(void*);
    void startTask();
  
};

#endif
