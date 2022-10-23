#ifndef MqttService_h
#define MqttService_h

#include "Config.h"
#include "Global.h"
#define CONDICIONADOR  "AR_CONDICIONADO"
#define LUZES "LUZES"
#define ATUALIZAR "ATUALIZAR_HORARIOS"

class MqttService 
{
  private:
    static bool __messageReturned;
    static String __message;
    static String getMessageToSend(MonitoringRequest request);
    static MonitoringRequest deserealizeObject(String payload);
    static void callback(char* topic, byte* payload, unsigned int length);
    static void setup();
    static void reconnect();

  public: 
    MqttService(PubSubClient *client);
    MqttService();
    
    String getMessage();
    void setMessage(String message);

    bool getMessageReturned();
    void setMessageReturned(bool messageReturned);

    void monitorSolicitation();
    static void executeSolicitation(MonitoringRequest solicitacao); 

    // Solicitacao
    void startMqttService(PubSubClient *client, HardwareRecord Hardware);
    static bool connectToActuator(String uuidDevice);
};

#endif
