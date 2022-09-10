#ifndef AwaitHttpService_h
#define AwaitHttpService_h

#include "Config.h"

#define CONDICIONADOR  "CONDICIONADOR"
#define LUZES "LUZES"
#define ATUALIZAR "ATUALIZAR_HORARIOS"

class AwaitHttpService 
{
  private: 
    static bool __messageReturned;
    static String __message;
    static void awaitsReturn();
    static String getMessageToSend(Solicitacao request);
    static MonitoringRequest deserealizeObject(String payload);

  public: 
    AwaitHttpService();

    String getMessage();
    void setMessage(String message);

    bool getMessageReturned();
    void setMessageReturned(bool messageReturned);

    static void awaitSolicitation();
    static void executeSolicitation(Solicitacao solicitacao); 

    // Solicitacao
    void startAwait();
    static bool connectToActuator(String uuidDevice); 

};

#endif
