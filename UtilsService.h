#ifndef UtilsService_h
#define UtilsService_h

#include "Config.h"

#define AC_ON  "AC-ON"
#define AC_OFF "AC-OFF" 
#define LZ_ON  "LZ-ON" 
#define LZ_OFF "LZ-OFF" 
#define TYPE_ACTUATOR 2
#define TYPE_SENSOR 3

class UtilsService
{
    public:
        UtilsService();
        String mountPayload(String deviceType, String state, String command);
        void updateMonitoring(String message);
};

#endif
