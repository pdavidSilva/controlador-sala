#include "Config.h"
#include "AwaitHttpService.h"

Config __configAcess;
BLEServerService* __bleConfiguration; 
HTTPService __httpService;
EnvironmentVariablesService __environment;
UtilsService __utils;

bool AwaitHttpService::__messageReturned = false;
String AwaitHttpService::__message = "";

AwaitHttpService::AwaitHttpService() {}

String AwaitHttpService::getMessage() {
    return  __message;
}

void AwaitHttpService::setMessage(String message) {
    __message = message;
}

bool AwaitHttpService::getMessageReturned() {
    return  __messageReturned;
}

void AwaitHttpService::setMessageReturned(bool messageReturned) {
    __messageReturned = messageReturned;
}


void AwaitHttpService::startAwait()
{
    // xTaskCreate(this->awaitSolicitation, "awaitSolicitation", 8192, this, 5, NULL);
}

void AwaitHttpService::awaitSolicitation()
{
    Solicitacao solicitacao;
    while (true)
    {
        if (__configAcess.isDebug())
        {
            Serial.println("=======================================");
            Serial.println("[AwaitHttpService] Start");
        }

        solicitacao = __httpService.getSolicitacao(LUZES);
        if (solicitacao.id != 0){
            executeSolicitation(solicitacao);
        }

        solicitacao = __httpService.getSolicitacao(CONDICIONADOR);
        if (solicitacao.id != 0){
            executeSolicitation(solicitacao);
        }

        if (__configAcess.isDebug())
        {
            Serial.println("=======================================");
            Serial.println("[AwaitHttpService] End");
        }
        delay(500);
    }
    
}

bool AwaitHttpService::connectToActuator(String uuidDevice) 
{
  bool deviceConnected = false;
  int i = 0;
  int count = 5;
            
  do
  { 
    i++;
    
    if (__configAcess.isDebug())
    {
      Serial.print("[AwaitHttpService]: attempt number: ");
      Serial.println(i);
    }
    
    deviceConnected = __bleConfiguration->connectToActuator(uuidDevice);
    
    if(deviceConnected)
      break;
      
  } while(i < count);

  if( i >= count && !deviceConnected)
      Serial.println("[AwaitHttpService]: device not found");

  return deviceConnected;
}

void AwaitHttpService::executeSolicitation(Solicitacao request) 
{
    __bleConfiguration->setReceivedRequest(true);
    bool dispConnected = connectToActuator(request.uuid);

    if(dispConnected)
    {
        String payload = getMessageToSend(request);
        __bleConfiguration->sendMessageToActuator(payload);

        awaitsReturn();

        __bleConfiguration->disconnectToActuator();
    }

    __bleConfiguration->setReceivedRequest(false);

    __utils.updateMonitoring(__message);

    if (__configAcess.isDebug())
    {
        Serial.println("==================================");
        Serial.println("[AwaitHttpService] Resposta BLE");
        Serial.println("[AwaitHttpService] recebeu retorno: " + __messageReturned);
        Serial.println("[AwaitHttpService] mensagem: " + __message);
    }

    __messageReturned = false;
    __message = "";  

}

String AwaitHttpService::getMessageToSend(Solicitacao request)
{
    String typeEquipament = "", state = "", command = "null";

    if(request.type == LUZES)
        typeEquipament = "LZ";
    else
    {
        typeEquipament = "AC";
        command = request.code;
    }

    if(request.acting == "True")
        state = "ON";
    else
        state = "OFF";

    return __utils.mountPayload(typeEquipament, state, command);
}

void AwaitHttpService::awaitsReturn()
{
  
  unsigned long tempoLimite = millis() + 15000;
  while(millis() <= tempoLimite && !__messageReturned)
  { 
      delay(1000);
      if (__configAcess.isDebug())
      {    
        Serial.print("[AwaitHttpService] TIME AWAITS: ");
        Serial.println(millis());
      }
  }    
}