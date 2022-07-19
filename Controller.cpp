#include "Config.h"
#include "Controller.h"
#include "BLESensorService.h"

Controller::Controller(){}
HTTPService __http;
ClientSocketService __clientSocketService;
BLEServerService* __bleConfig; 
EnvironmentVariablesService __environmentService;
Config __config; 

bool Controller::start(HardwareRecord &record) const 
{   
    struct HardwareRecord hardware = {0,"",""};

    __http.getInfoHardware(hardware);
    if(hardware.id > 0 ){
        record = hardware;

        return true;
    }else{
        return false;
    }
}

void Controller::restart() const 
{
  Hardware hardware;
  hardware.restart();

  return ;
}

bool Controller::registerHardware(HardwareRecord hardware) const
{   
    bool statusRegister = false;

    statusRegister = __http.registerHardware(hardware);
    if(statusRegister){
        return true;
    }
    
    return false;
}
        
bool Controller::notificateServer() const
{
    return true;
}

void Controller::initBLETaskServer()
{
    delay(2000);
    __bleConfig->startTaskBLE();
}

void Controller::configureBLEServer()
{
    __bleConfig->initBLE();  
    __bleConfig->scanDevices();
    __bleConfig->populateMap();
}

void Controller::configureBLEClient()
{
   initBLEClient();  
}

void Controller::getSensors(HardwareRecord hardware, String sensors[], int &indexSensor)
{
    //__http.getSensors(hardware, sensors, indexSensor);
    return;
}

bool Controller::getMaster(HardwareRecord hardware, String &master)
{
    __http.getMaster(hardware, master);
    return !master.equals("") ? true : false;


}

 void Controller::sendDataOfMonitoring(MonitoringRecord monitoringRecord)
{
    DynamicJsonDocument doc(1024);
    String data;
    doc['temperature'] = monitoringRecord.temperature;
    doc['hasPresent'] = monitoringRecord.hasPresent;
    Serial.println("Controller -> sendDataOfMonitoring");
    serializeJson(doc, data);
    sendDataToServer(data);
    delay(3000);
  }

void Controller::initServerSocket()
{
  __clientSocketService.initServer();
}

void Controller::startTaskWebSocket()
{  
    __clientSocketService.startTaskWebSocket();
}

HardwareRecord Controller::getHardwareConfig()
{
    return __environmentService.getHardware();
}

void Controller::setHardwareConfig(HardwareRecord hardware)
{
    __environmentService.setHardware(hardware);
}

void Controller::environmentVariablesContinuousValidation()
{
    __environmentService.continuousValidation();
}

void Controller::initEnvironmentVariables()
{
    __environmentService.initEnvironmentVariables();
}

void Controller::fillHardwares(HardwareRecord hardware)
{
    std::vector<struct HardwareRecord> hardwares = __http.getHardwares(hardware);
        
    for(struct HardwareRecord r : hardwares)
    {
        if(r.typeHardwareId == TYPE_SENSOR)
            __bleConfig->addSensor(r.uuid);
        else
            __bleConfig->addActuator(r);
    } 
}

bool Controller::loadedDevices()
{
    if ( __bleConfig->getActuators().size() > 0 && __bleConfig->getSensors().size() > 0)
        return true;

    return false;
}
