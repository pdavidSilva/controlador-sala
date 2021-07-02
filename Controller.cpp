#include "Config.h"
#include "Controller.h"
 
Controller::Controller(){}
HTTPService __http;

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
