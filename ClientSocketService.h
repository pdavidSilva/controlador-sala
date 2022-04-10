#ifndef ClientSocketService_h
#define ClientSocketService_h

#include "Config.h"
#include <IRsend.h>

#define CONDICIONADOR  "CONDICIONADOR"
#define LUZES "LUZES"
#define ATUALIZAR "atualizarHorarios;"


class ClientSocketService 
{
  private: 
    static bool __messageReturned;
    static String __message;
    static WiFiServer __server;

    static int tratarMsgRecebida(String msg);
    static String SplitGetIndex(String data, char separator, int index);

    static void awaitsReturn();


  public: 
    ClientSocketService();

    WiFiClient getClient();
    void setClient(WiFiClient client);

    String getMessage();
    void setMessage(String message);

    bool getMessageReturned();
    void setMessageReturned(bool messageReturned);

    void initServer();

    static void recebeComandosDoServidor(void *arg);

};

#endif
