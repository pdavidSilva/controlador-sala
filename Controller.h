#ifndef Controller_h
#define Controller_h

#include "Config.h"

class Controller
{
    
    public:
        Controller();
        bool start() const;
        void restart() const;
        bool registerHardware() const;
        bool notificateServer() const;       

        void initServer();    
        
};

#endif
