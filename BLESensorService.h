#ifndef BLESensorService_h
#define BLESensorService_h

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Arduino.h>
#include <String>

#define CHARACTERISTICUUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define SERVICEUUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define GETDATA  "GETDATA"
#define LED 2

static BLECharacteristic* pCharacteristicSensor;  
static bool deviceConnected;
static BLEServer* pServer;
static bool sendData;

class MyServerCallbacks: 
    public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {

      digitalWrite(LED, HIGH);
      pCharacteristicSensor->setValue("4fafc201-1fb5-459e-8fcc-c5c9c3319222");
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

    Serial.println("RECEBEU DADO");
    std::string sensoriamento = pCharacteristic->getValue();
    Serial.println(sensoriamento.c_str());

    if(String(GETDATA).equals(sensoriamento.c_str()))
       sendData = true;       
  }
};

void initBLE()
{
  pinMode(LED, OUTPUT);

  BLEDevice::init("ESP32");

  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICEUUID);

  pCharacteristicSensor = pService->createCharacteristic(
                      CHARACTERISTICUUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

  pCharacteristicSensor->addDescriptor(new BLE2902());

  pCharacteristicSensor->setCallbacks(new MyCallbacks());

  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICEUUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);
  BLEDevice::startAdvertising();

  Serial.println("Waiting a client connection to notify...");
}

void sendDataOfMonitoring()
{
  if (deviceConnected) 
  {
      if(sendData)
      {
        String data = "Meus disp 111";
        pCharacteristicSensor->setValue(data.c_str());
        pCharacteristicSensor->notify();
        delay(1500); 
      }
  }
}
    
#endif
