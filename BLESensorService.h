#ifndef BLESensorService_h
#define BLESensorService_h

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Arduino.h>
#include <String>
#include "EquipmentService.h"

#define CHARACTERISTICUUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define SERVICEUUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define GETDATA  "GETDATA"
#define LED 2

static BLECharacteristic* pCharacteristicSensor;  
static bool deviceConnected;
static BLEServer* pServer;
static bool sendData;
EnvironmentVariablesService __environmentVariableService;
static DeviceType deviceType;
EquipmentService equipmentService;
static String equipmentState = "";

void sendDataToServer(String data)
{
  if (deviceConnected) 
  {
      if(sendData)
      {
        pCharacteristicSensor->setValue(data.c_str());
        pCharacteristicSensor->notify();
        delay(1500); 
      }
  }
}

class MyServerCallbacks: 
    public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {

      digitalWrite(LED, HIGH);
      pCharacteristicSensor->setValue(__environmentVariableService.getHardware().uuid.c_str());
      pCharacteristicSensor->notify();
      
      deviceConnected = true;
      sendData = false;

      Serial.println("CONECTADO");
            
      delay(1000);
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;      
      digitalWrite(LED, LOW);
      Serial.println("DESCONECTADO");

      ESP.restart();
    }
};


class MyCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic * pCharacteristic) {

    std::string response = pCharacteristic->getValue();
    Serial.println(response.c_str());

    if(String(GETDATA).equals(response.c_str())) {
       sendData = true;       
    } else if(deviceType != NULL && deviceType == ATUADOR){
      Serial.println("ATUADOR - (ONWRITE) COMMANDO PARA O EQUIPAMENTO");
      equipmentState = equipmentService.executeActionFromController(response.c_str());
      sendData = true;
      sendDataToServer(equipmentState.c_str());
      sendData = false;
      equipmentState = "";
    }
  }
};

void initBLEClient(String deviceName, DeviceType devType)
{
  pinMode(LED, OUTPUT);
  deviceType = devType;
  
  Serial.println("========================================");
  Serial.println("[BLE_CLIENT]: Set Name Disp");
  BLEDevice::init(std::string(deviceName.c_str()));

  Serial.println("[BLE_CLIENT]: Create server");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  Serial.println("[BLE_CLIENT]: Create Service");
  BLEService *pService = pServer->createService(SERVICEUUID);

  Serial.println("[BLE_CLIENT]: Create Characteristic");
  pCharacteristicSensor = pService->createCharacteristic(
                      CHARACTERISTICUUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

  pCharacteristicSensor->addDescriptor(new BLE2902());

  pCharacteristicSensor->setCallbacks(new MyCallbacks());

  Serial.println("[BLE_CLIENT]: Start Service");
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICEUUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x0);
  BLEDevice::startAdvertising();

  Serial.println("Waiting a client connection to notify...");
}
    
#endif
