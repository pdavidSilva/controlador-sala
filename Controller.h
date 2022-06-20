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
        void fillHardwares(struct HardwareRecord);


        // BLE
        void configureClient();   
        void configureServer();   
        void initBleTaskServer();    
        bool notificateServer() const;       

        //send Data Monitoring
        void sendDataOfMonitoring(struct MonitoringRecord);

        // WEB SOCKET
        void initServerSocket();    
        void startTaskWebSocket(); 

        HardwareRecord getHardwareConfig();
        void setHardwareConfig(HardwareRecord hardware);

        // Validacao continua de variaveis de ambiente
        void environmentVariablesContinuousValidation();
        void initEnvironmentVariables();
};

#endif
