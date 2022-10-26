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

        bool getMaster(struct HardwareRecord, String &);
        void fillHardwares(struct HardwareRecord);

        bool loadedDevices();

        // BLE
        void configureClient(String deviceName, DeviceType deviceType);   
        void configureBLEServer();   
        void initBLETaskServer();    
        bool notificateServer() const;       

        //send Data Monitoring
        void sendDataMonitoring(struct MonitoringRecord);

        // WEB SOCKET
        void initServerSocket();    
        void startTaskWebSocket();
        void startTaskMqtt(PubSubClient *client, HardwareRecord hardware);
        HardwareRecord getHardwareConfig();
        void setHardwareConfig(HardwareRecord hardware);

        // Validacao continua de variaveis de ambiente
        void environmentVariablesContinuousValidation();
        void initEnvironmentVariables();

        // Send Commands
        void ExecuteCommandIR(String command);
};

#endif
