#ifndef MqttService_h
#define MqttService_h

#include "Config.h"

#define CONDICIONADOR  "AR_CONDICIONADO"
#define LUZES "LUZES"
#define ATUALIZAR "ATUALIZAR_HORARIOS"

class MqttService 
{
  private:
    static bool __messageReturned;
    static String __message;
    static WiFiClientSecure __espClient;
    static PubSubClient * __client;
    static String getMessageToSend(MonitoringRequest request);
    static MonitoringRequest deserealizeObject(String payload);
    static void callback(char* topic, byte* payload, unsigned int length);
    static void setup();
    static void reconnect();

  public: 
    MqttService();
    
    String getMessage();
    void setMessage(String message);

    bool getMessageReturned();
    void setMessageReturned(bool messageReturned);

    static void monitorSolicitation(void* _this);
    static void executeSolicitation(MonitoringRequest solicitacao); 

    // Solicitacao
    void startMqttService(HardwareRecord Hardware);
    static bool connectToActuator(String uuidDevice);
};

#endif
