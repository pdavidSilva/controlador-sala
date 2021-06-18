#include "Config.h"
#include "Controller.h"

HTTPService __http;

Controller::Controller(){}

bool Controller::start() const 
{   
    struct HardwareRecord hardware = {0,"",""};

    __http.getInfoHardware(hardware);
    if(hardware.id > 0 ){
        //this->setId(hardware.id);
        //this->setUuid(hardware.uuid);
        //this->setToken(hardware.token);

        return true;
    }else{
        return false;
    }
}

void Controller::restart() const 
{
  
}


bool Controller::registerHardware() const
{
    return true;
}
        
bool Controller::notificateServer() const
{
    return true;
}

void Controller::initServer()
{
  
}
