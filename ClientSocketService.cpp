#include "Config.h"
#include "ClientSocketService.h"

Config configuration;
BLEServerService* __bleConfiguration; 
ClientSocketService::ClientSocketService() {}

bool ClientSocketService::__messageReturned = false;
String ClientSocketService::__message = "";
WiFiServer ClientSocketService::__server(8088);

void ClientSocketService::initServer() {
      __server.begin();
}

String ClientSocketService::getMessage() {
    return  __message;
}

void ClientSocketService::setMessage(String message) {
    __message = message;
}

bool ClientSocketService::getMessageReturned() {
    return  __messageReturned;
}

void ClientSocketService::setMessageReturned(bool messageReturned) {
    __messageReturned = messageReturned;
}

/*
 * <descricao> Ouve requisicoes do cliente conecta via socket <descricao/>
 */
void ClientSocketService::recebeComandosDoServidor(void *arg) {

    WiFiClient client;    
    
    while(true) {          
      /* 
       * ouvindo o cliente 
       */
      client = __server.available();
  
      if (client) {
  
        /*
         * Checando se o cliente está conectando ao server
         */
        while (client.connected()) {
  
          if (client.available()) {
            String && msg = client.readStringUntil('\n');
            
            if (configuration.isDebug())
            {
                Serial.println("==================================");
                Serial.println("[ClientSocketService] mensagem recebida");
                Serial.println("[ClientSocketService] mensagem: " + msg);
            }

            int tipoMensagem = tratarMsgRecebida(msg);
            delay(1000);
            
            __bleConfiguration->setReceivedRequest(true);

            if(connectToActuator())
              __bleConfiguration->sendMessageToActuator(msg);

            awaitsReturn();

            __bleConfiguration->disconnectToActuator();
           
            if (tipoMensagem == (-1) || tipoMensagem == (-2)) { 
                  
                  if(__messageReturned)   
                    client.println(__message);
                  else
                    client.println("NOT-AVALIABLE");

                  __messageReturned = false;
                  __message = "";  

                  if (configuration.isDebug())
                  {
                    Serial.println("==================================");
                    Serial.println("[ClientSocketService] Resposta BLE");
                    Serial.println("[ClientSocketService] recebeu retorno: " + __messageReturned);
                    Serial.println("[ClientSocketService] mensagem: " + __message);
                  }                     

            }  else if(tipoMensagem == (-3)) {

                  client.println("OK");
            
            }
          }  
          delay(100);
        }
      }
      delay(500);
    }
}

bool ClientSocketService::connectToActuator() 
{
  bool connected = false;
  int i = 0, cont = 3;
            
  while(i < cont || !connected)
  { 
    connected = __bleConfiguration.connectToActuator(msg);
    i++;
  }

  return connected;
}


/*
 * <descricao> Esse metodo retorna o codigo IR e por referencia atribui o nome do dispositivo <descricao/>
 * <parametros> msg: codigos IR recebidos na requisicao do servidor <parametros/>
 * <retorno> Lista de inteiros com codigos ir <retorno/>
 */
int ClientSocketService::tratarMsgRecebida(String msg) {
  
  String tipoDeMsg = SplitGetIndex(msg, ';', 0);
  int retorno = 0;
  
  
  if (tipoDeMsg == CONDICIONADOR) { 
   
    //enviarMonitoramento(luzesLigadas, arLigado);
        
    //String logMonitoramento = arLigado ? "Ligando luzes no horario: " + horaAtualSistema :  "Desligando luzes no horario: " + horaAtualSistema;
    //gravarLinhaEmArquivo(SPIFFS, logMonitoramento, pathLogMonitoramento); 
    
    retorno = -1;    
    
  } else if (tipoDeMsg == LUZES) {
    
    //enviarMonitoramento(luzesLigadas, arLigado);

    retorno = -2;  
    
  } else if (tipoDeMsg == ATUALIZAR) {

    //obterHorariosDaSemana();
    //carregarHorariosDeHojeDoArquivo(SPIFFS, obterDataServidor("GETDATE"));

    retorno = -3;
  }

  return retorno;
}


/*
 * <descricao> Obtem nome do dispositivo ou os codigos IR neviados na requisicao do servidor  <descricao/>
 * <parametros> data: codigos IR recebidos na requisicao do servidor <parametros/>
 * <parametros> separator: caracter chave para realizar o 'split' <parametros/>
 * <parametros> index: identificar que diz se quem chama quer receber o nome do dispositivo ou os codigos IR <parametros/>
 * <retorno> string com nome do dispotivo recebido na requisicao ou os codigos IR <retorno/>
 */
String ClientSocketService::SplitGetIndex(String data, char separator, int index) 
{
  int found = 0;
  int strIndex[] = {
    0,
    -1
  };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) 
  {
    if (data.charAt(i) == separator || i == maxIndex) 
    {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}


void ClientSocketService::awaitsReturn()
{
  
  unsigned long tempoLimite = millis() + 15000;
  while(millis() <= tempoLimite && !__messageReturned){ }
  
}
