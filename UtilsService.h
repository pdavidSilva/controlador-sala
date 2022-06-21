#ifndef UtilsService_h
#define UtilsService_h

#include "Config.h"

#define AC_ON  "AC_ON"
#define AC_OFF "AC_OFF" 
#define LZ_ON  "LZ_ON" 
#define LZ_OFF "LZ_OFF" 

class UtilsService
{
    public:
        UtilsService();
        String mountPayload(String deviceType, String state, String command);
        void updateMonitoring(String message);
};

#endif
