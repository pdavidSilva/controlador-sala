#include "BLEServerService.h"

int __countTypeSensor;
int __countTypeActuator;
vector<String> __sensors;
vector<String> __actuators;
bool __receivedRequest;
BLEScan* __pBLEScan;
vector<BLEAdvertisedDevice*> __filteredDevices;
unordered_map<string, Hardware> __devicesMapped;

BLEServerService::BLEServerService()
{
    __countTypeSensor = 0;
    __countTypeActuator = 0;
    __receivedRequest = false;
    __pBLEScan = BLEDevice::getScan();
}

void BLEServerService::notifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify) 
{
    if (isNotify) 
    {
      Serial.print("Notify callback for characteristic: ");
      Serial.println(pBLERemoteCharacteristic->getUUID().toString().c_str());
      Serial.print(" of data length ");
      Serial.println(length);
      Serial.print("data: ");
      String data = String(((char*)pData));
      Serial.println(data.substring(0, length));
    }
}
  
BLEDeviceConnect* BLEServerService::connectToDevice(BLEAdvertisedDevice* myDevice, bool validateConnection) 
{
    BLEDeviceConnect* device = new BLEDeviceConnect();
    device->deviceFound = true;
  
    delay(250);
    device->pClient = BLEDevice::createClient();
    Serial.println("[CONNECTION]: - Created client");
  
    delay(250);
    bool ok = device->pClient->connect(myDevice);
  
    if(ok)
    {
        Serial.println("[CONNECTION]: - Connected to disp");
  
        delay(250);
        device->pRemoteService = device->pClient->getService(SERVICE_UUID);
        if (device->pRemoteService == nullptr) 
        {
          Serial.print("[CONNECTION]: Failed to find our service UUID: ");
          Serial.println(SERVICE_UUID.toString().c_str());
          
          device->pClient->disconnect();
          device->deviceFound = false;
          
          return device;
        }
      
        Serial.println("[CONNECTION]: - Found our service");
  
        delay(250);
        device->pRemoteCharacteristic = device->pRemoteService->getCharacteristic(CHARACTERISTIC_UUID);
        if (device->pRemoteCharacteristic == nullptr) 
        {
          Serial.print("[CONNECTION]: Failed to find our characteristic UUID: ");
          Serial.println(CHARACTERISTIC_UUID.toString().c_str());
          
          device->pClient->disconnect();
          device->deviceFound = false;
          
          return device;
        }
      
        Serial.println("[CONNECTION]: - Found our characteristic");
      
        if (validateConnection)
        {       
              delay(250);
              if(device->pRemoteCharacteristic->canRead())
              {
                  delay(250);
                  std::string value = device->pRemoteCharacteristic->readValue();
              
                  Serial.print("[CONNECTION]: uuid pesquisado: ");
                  Serial.println(value.c_str());
      
                  if(!isAtuador(value.c_str()) && !isSensor(value.c_str()))
                  {
                     Serial.println("[CONNECTION]: device not found ");
                     device->pClient->disconnect();
                     device->deviceFound = false;
                     
                     return device;
                  }
      
                  device->uuid = value.c_str();
                  Serial.print("[CONNECTION]: uuid the device: ");
                  Serial.println(device->uuid.c_str());
              }
              else
              {
                 Serial.println("[CONNECTION]: device not found ");      
                 device->pClient->disconnect();
                 device->deviceFound = false;
                          
                 return device;
              }  
        } 
      
        if(device->pRemoteCharacteristic->canNotify())
          device->pRemoteCharacteristic->registerForNotify(notifyCallback);
        
        Serial.println("[CONNECTION]: dispositivo conectado ");
        Serial.print("[CONNECTION]: ");
        Serial.println(CHARACTERISTIC_UUID.toString().c_str());
        Serial.print("[CONNECTION]: ");
        Serial.println(myDevice->getAddress().toString().c_str());
  
        Serial.print("[CONNID]: ");
        Serial.println(device->pClient->getConnId());
        Serial.print("[RSSI]: ");
        Serial.println(device->pClient->getRssi());
    }
    else
        device->deviceFound = false; 
  
    return device;
}
  
void BLEServerService::initBLE() 
{
    Serial.println("[BLEServerSettings] Iniciando configuracoes BLE");
    BLEDevice::init("ESP32_CONTROLLER");
    __pBLEScan->setInterval(1349);
    __pBLEScan->setWindow(449);
    __pBLEScan->setActiveScan(true);
}
  
void BLEServerService::scanDevices() 
{
    BLEScanResults foundDevices = __pBLEScan->start(5, false);
  
    for (int i = 0; i < foundDevices.getCount(); i++)
      __filteredDevices.push_back(new BLEAdvertisedDevice(foundDevices.getDevice(i)));
  
    BLEAdvertisedDevice* disp;
    for (disp : __filteredDevices)
      Serial.println(disp->toString().c_str());
}
  
/*
     Após fazer a busca, ele faz uma filtragem de quais dispositivos devem ser
*/
void BLEServerService::populateMap() 
{
   for (auto disp : __filteredDevices) 
   {
        if(disp->haveServiceUUID() && disp->isAdvertisingService(SERVICE_UUID))
        {
          connectMyDisp(disp);
          Serial.println("[CONNECTION]: Device Found");
        }
        else
        {
           Serial.println();
           Serial.println("[CONNECTION]: Device no listed");
        }
    }
  
    if (__devicesMapped.size() == 0)
      Serial.println("[CONNECTION]: no devices found");    
}
  
bool BLEServerService::isSensor(String uuid)
{ 
    if(std::count(__sensors.begin(), __sensors.end(), uuid))
      return true;
  
    return false;
}
  
bool BLEServerService::isAtuador(String uuid)
{ 
  if(std::count(__actuators.begin(), __actuators.end(), uuid))
      return true;
  
  return false;
}
  
bool BLEServerService::connectMyDisp(BLEAdvertisedDevice* device) 
{
    Serial.println();
    Serial.println("[CONNECTION]: connecting to my devices...");
  
    BLEDeviceConnect *deviceConnected = connectToDevice(device, true);
  
    if (deviceConnected->deviceFound)
    {
      Serial.println("[CONNECTION]: forming successfull a connection");
  
      Hardware disp;
      disp.setBLEAdvertisedDevice(device);
      disp.setUuid(deviceConnected->uuid.c_str());

      if(isSensor(deviceConnected->uuid.c_str()))
      {
        Serial.println("[CONNECTION]: é sensor ");
        disp.setTypeDisp(TYPE_SENSOR);
        __countTypeSensor++;
      }
      else
      {
        Serial.println("[CONNECTION]: é atuador ");
        disp.setTypeDisp(TYPE_ACTUATOR);
        __countTypeActuator++;
      }
  
      __devicesMapped.insert(std::make_pair(deviceConnected->uuid.c_str(), disp));
      Serial.println("[CONNECTION]: connection saved. finalized");
  
      if(deviceConnected->pClient->isConnected())    
        deviceConnected->pClient->disconnect(); 
        
      free(deviceConnected->pClient);
      free(deviceConnected->pRemoteCharacteristic);
      free(deviceConnected->pRemoteService);
      delete deviceConnected;
      
      return true;
    }
    else
    {
      Serial.println("[CONNECTION]: have problems in connection");
      
      free(deviceConnected->pClient);
      free(deviceConnected->pRemoteCharacteristic);
      free(deviceConnected->pRemoteService);
      delete deviceConnected;
      
      return false;
    }
}
  
void BLEServerService::sendMessageToActuator(String data, String device) 
{    
    //Dispositivo disp;
    Hardware disp;
    BLEDeviceConnect *deviceConnected;
  
    for (auto item : __devicesMapped) 
    {
      disp = item.second;
      if (device.equals(disp.getMacAddressAdvertisedDevice().toString().c_str())) 
      {
        deviceConnected = connectToDevice(disp.getBLEAdvertisedDevice(), false);
        if(deviceConnected->pClient->isConnected())
          deviceConnected->pRemoteCharacteristic->writeValue(data.c_str(), data.length());
        
        delay(5000);
        deviceConnected->pClient->disconnect();
        delete deviceConnected;
      }
    }
}
  
  
void BLEServerService::continuousConnectionTask() 
{  
    BLEDeviceConnect *deviceConnected;
    vector <BLEDeviceConnect*> aux;
    bool allDisconected = true;
    //Dispositivo disp;
    Hardware disp;
    int count = 0;
  
    while (true)
    {
        Serial.println();
        Serial.println("[CONTINUOUS_CONNECTION]: NEW CICLE");
        for (auto item : __devicesMapped) 
        {
            if(!__receivedRequest)
            {
                disp = item.second;
                
                if(disp.getTypeDisp() == TYPE_SENSOR)
                {
                    Serial.println();
                    Serial.print("[CONTINUOUS_CONNECTION] UUID: ");
                    Serial.println(disp.getUuid());
  
                    Serial.print("[CONTINUOUS_CONNECTION] ADRRES: ");
                    Serial.println(disp.getBLEAdvertisedDevice()->getAddress().toString().c_str());
                    
                    deviceConnected = connectToDevice(disp.getBLEAdvertisedDevice(), false);
                    aux.push_back(deviceConnected);
                    count++;
                  
                    if ((count % 3) == 0 || __countTypeSensor == count)
                    {
                        for (auto deviceCon : aux)
                        {
                          if(deviceCon->pClient->isConnected())
                          {
                            String data = "GETDATA";
                            deviceCon->pRemoteCharacteristic->writeValue(data.c_str(), data.length());
                            allDisconected = false;
                          }
                        }
                         
                        // timer
                        if(!allDisconected)
                          timer();
                        
                        allDisconected = true;
                        
                        Serial.println();
                        for (auto deviceCon : aux) 
                        {
                            Serial.print("[DISCONNECT]: ");
                            Serial.println(deviceCon->pClient->getPeerAddress().toString().c_str());
                            
                            delay(300);
                            if(deviceCon->pClient->isConnected())
                              deviceCon->pClient->disconnect();
                              
                            delay(300);  
                            free(deviceCon->pClient);
                            free(deviceCon->pRemoteCharacteristic);
                            free(deviceCon->pRemoteService);
                            
                            delete deviceCon;
                        }
      
                        aux.clear();
                    }
                }
            }
            else
            {
               Serial.print("[CONTINUOUS_CONNECTION] request enabled ");
            }
        }
        count = 0;
        
        delay(500);
    }
}

vector<String> BLEServerService::getSensors()
{
  return __sensors;
}

vector<String> BLEServerService::getActuators()
{
  return __actuators;
}

void BLEServerService::setReceivedRequest(bool receivedRequest)
{
   __receivedRequest = receivedRequest;
}

bool BLEServerService::getReceivedRequest()
{
  return __receivedRequest;
}

void BLEServerService::setCountTypeSensor(int count)
{
  __countTypeSensor = count;
}

void BLEServerService::setCounttypeActuator(int count)
{
  __countTypeActuator = count;
}

int BLEServerService::getCountTypeSensor()
{
  return __countTypeSensor;
}

int BLEServerService::getCounttypeActuator()
{
  return __countTypeActuator;
}

void BLEServerService::addSensor(String uuid)
{
  __sensors.push_back(uuid);
}

void BLEServerService::addActuator(String uuid)
{
  __actuators.push_back(uuid);
}
  
void BLEServerService::timer()
{     
   unsigned long tempoLimite = millis() + TIME_CONNECTION;
    
   while(millis() <= tempoLimite && !__receivedRequest){
     
     if((millis() % 5000) == 0)
     {
       Serial.println();
       Serial.print("[MILLIS]: MARCAÇÃO TEMPORAL: ");
       Serial.println();
       Serial.print("[MILLIS]: tempo atual: ");
       Serial.println(millis());
       Serial.print("[MILLIS]: tempo limite: ");
       Serial.println(tempoLimite);
       Serial.println();
     }
   };
}

void BLEServerService::startTaskImpl(void* _this)
{
    BLEServerService* bleSettings = (BLEServerService*)_this;
    bleSettings->continuousConnectionTask();
}

void BLEServerService::startTask()
{
    xTaskCreate(this->startTaskImpl, "Task", 4096, this, 5, NULL);
}
