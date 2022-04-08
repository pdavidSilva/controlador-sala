#ifndef ClientSocketService_h
#define ClientSocketService_h

#include <IRsend.h>
#include <WiFi.h>

#define CONDICIONADOR  "CONDICIONADOR"
#define LUZES "LUZES"
#define ATUALIZAR "atualizarHorarios;"

WiFiServer server(8088);

class ClientSocketService 
{
  private: 
    bool __messageReturned;
    WiFiClient __client;
    String __message;

    int tratarMsgRecebida(String msg);
    String SplitGetIndex(String data, char separator, int index);

    void awaitsReturn();


  public: 
    ClientSocketService();

    WiFiClient getClient();
    void setClient(WiFiClient client);

    String getMessage();
    void setMessage(String message);

    bool getMessageReturned();
    void setMessageReturned(bool messageReturned);

    void recebeComandosDoServidor(void *arg);

}

#endif
