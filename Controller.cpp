#include "Config.h"
#include "Controller.h"
#include "BLESensorService.h"

Controller::Controller(){}
HTTPService __http;
ClientSocketService __clientSocketService;

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

void Controller::initServer()
{
  
}

void Controller::getSensors(HardwareRecord hardware, String sensors[], int &indexSensor)
{
    __http.getSensors(hardware, sensors, indexSensor);
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

    serializeJson(doc, data);
    sendDataBle(data);
    delay(3000);
  }

void Controller::initServerSocket()
{
  __clientSocketService.initServer();
}

void Controller::startTaskWebSocket()
{  
  xTaskCreatePinnedToCore(__clientSocketService.recebeComandosDoServidor, 
                        "ClientSocketService.recebeComandosDoServidor", 
                        10000, 
                        NULL, 
                        8, 
                        NULL, 
                        tskNO_AFFINITY);
}