#ifndef Controller_h
#define Controller_h

#include "Config.h"


class Controller
{   
    public:
        Controller();
        void restart() const;

        // HTTP
        bool start(struct HardwareRecord&) const;
        bool registerHardware(struct HardwareRecord) const;
        void getSensors(struct HardwareRecord, String *, int&);
        //void getDevices(String[]&);

        // BLE
        void initServer();    
        bool notificateServer() const;       
        
};

#endif
