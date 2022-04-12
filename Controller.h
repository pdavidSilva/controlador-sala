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
        bool getMaster(struct HardwareRecord, String &);

        // BLE
        void configureServer();   
        void initBleTaskServer();    
        bool notificateServer() const;       

        //send Data Monitoring
        void sendDataOfMonitoring(struct MonitoringRecord);

         // WEB SOCKET
        void initServerSocket();    
        void startTaskWebSocket(); 
};

#endif

