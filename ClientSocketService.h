#ifndef ClientSocketService_h
#define ClientSocketService_h

#define CONDICIONADOR  "CONDICIONADOR"
#define LUZES "LUZES"
#define ATUALIZAR "atualizarHorarios;"
  
class ClientSocketService 
{
  private: 
    bool __messageReturned;
    WiFiClient __client;
    String __message;

    int tratarMsgRecebida(String msg);
    String SplitGetIndex(String data, char separator, int index);

  public: 

    WiFiClient getClient();
    void setClient(WiFiClient client);

    void recebeComandosDoServidor(void *arg);

}

#endif