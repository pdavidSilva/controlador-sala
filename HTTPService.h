#ifndef HTTPService_h
#define HTTPService_h

#include "Config.h"

class HTTPService
{
    public :
        HTTPService();
        void getInfoHardware(struct HardwareRecord&);
        bool registerHardware(struct HardwareRecord);
        void getSensors(struct HardwareRecord, String [], int &);
        void getDevices(struct HardwareRecord, String [], int &);
            
};

#endif
