#include "EnvironmentVariablesService.h"

#include <NTPClient.h>
#include <WiFiUDP.h>

String __currentTime;
struct Monitoramento __monitoring;
vector<struct Reserva> EnvironmentVariablesService::__reservations; 
HardwareRecord EnvironmentVariablesService::__hardware; 
String __startTimeLoadReservations;
String __endTimeLoadReservations;
bool EnvironmentVariablesService::__uploadedToday;
bool EnvironmentVariablesService::__receivedData;
bool EnvironmentVariablesService::__hasMovement;
String EnvironmentVariablesService::__message;
WiFiUDP __udp;
NTPClient __ntp(__udp, "a.st1.ntp.br", -3 * 3600, 60000);

BLEServerService* __bleServerConfig;
HTTPService __httpRequestService;

EnvironmentVariablesService::EnvironmentVariablesService()
{
    __startTimeLoadReservations  = "00:05:00";
    __endTimeLoadReservations    = "00:10:00";
    __uploadedToday = false;
    __ntp.begin();
    __ntp.forceUpdate();
}

String EnvironmentVariablesService::getMessage() 
{
    return __message;
}

void EnvironmentVariablesService::setMessage(String message) 
{
    __message = message;
}

bool EnvironmentVariablesService::getReceivedData() 
{
    return __receivedData;
}

void EnvironmentVariablesService::setReceivedData(bool receivedData) 
{
    __receivedData = receivedData;
}

std::vector<struct Reserva> EnvironmentVariablesService::getReservations()
{
    return __reservations;
}

void EnvironmentVariablesService::setReservations(std::vector<struct Reserva> reservations)
{
    __reservations = reservations;
}

struct HardwareRecord EnvironmentVariablesService::getHardware()
{
    return __hardware;
}

void EnvironmentVariablesService::setHardware(HardwareRecord hardware)
{
    __hardware = hardware;
}

String EnvironmentVariablesService::getCurrentTime()
{
    return __currentTime;
}

String EnvironmentVariablesService::setCurrentTime(String currentTime)
{
  __currentTime = currentTime;
}

struct Monitoramento EnvironmentVariablesService::getMonitoring()
{
  return __monitoring;
}

void EnvironmentVariablesService::setMonitoring(struct Monitoramento monitoring)
{
  __monitoring = monitoring;
}

void EnvironmentVariablesService::sendDataToActuator(String uuid, String message)
{
  __bleServerConfig->setReceivedRequest(true);
  __bleServerConfig->setEnvironmentSolicitation(true);

  bool dispConnected = __bleServerConfig->connectToActuator(uuid);
                
  if(dispConnected)
  {
      __bleServerConfig->sendMessageToActuator(message);

      awaitsReturn();

      __bleServerConfig->disconnectToActuator();
  }
                
  __bleServerConfig->setEnvironmentSolicitation(false);
  __bleServerConfig->setReceivedRequest(false);
} 

void EnvironmentVariablesService::sendDataToActuator(int typeEquipment, String message)
{
  for(struct HardwareRecord r : __bleServerConfig->getActuators())
  {
    if(r.typeEquipment == typeEquipment)
      sendDataToActuator(r.uuid, message);
  }
}

/*
 * <descricao> Verifica se é para ligar os dispostivos (luzes e ar) de acordo com as 
 * infomacoes obtidas dos modulos de sensoriamento e dos dados das reservas da sala <descricao/>
 */
void EnvironmentVariablesService::turnOnManagedDevices() {
  String horaInicio, horaFim, logMonitoramento;
  
  struct Reserva r;
  for (r: __reservations) {

    horaInicio = r.horarioInicio;
    horaFim = r.horarioFim;
    
    if (__currentTime >= r.horarioInicio && __currentTime < r.horarioFim && __hasMovement) {

      if (!__monitoring.conditioner)
        turnOnConditioner();

      if (!__monitoring.light)
        turnOnLight();
    }
  }
}

/*
 * <descricao> Verifica se é para desligar os dispostivos (luzes e ar) de acordo com as 
 * informacoes obtidas dos modulos de sensoriamento e dos dados das reservas da sala <descricao/>
 */
void EnvironmentVariablesService::turnOffManagedDevices() {
  String horaInicio;
  String horaFim;
  String logMonitoramento;
  bool notInClass = true;

  struct Reserva r;
  for (r: __reservations) {

    horaInicio = r.horarioInicio;
    horaFim = r.horarioFim;

    if (__currentTime >= r.horarioInicio && __currentTime < r.horarioFim)
      notInClass = false;
  }

  if (notInClass) {
    if (__monitoring.conditioner) 
      turnOfConditioner();

    if (__monitoring.light)
      turnOfLight();
  }
}

/*
 * <descricao> Executa o comando de ligar luzes e envia o status do monitoramento pra o servidor além de gravar a operação em log <descricao/>
 */
void EnvironmentVariablesService::turnOnConditioner(){

  Serial.println("LIGANDO CONDICIONADOR");

  String codigos = __httpRequestService.getComandosIrByIdSalaAndOperacao();

  //------------------------------------------------------    
  String payload = mountPayload("AC", "ON", codigos);
  sendDataToActuator(TYPE_CONDITIONER, payload);
  //------------------------------------------------------

  __monitoring.conditioner = true;

  __httpRequestService.putMonitoring(__monitoring);
}

/*
 * <descricao> Executa o comando de desligar luzes e envia o status do monitoramento pra o servidor além de gravar a operação em log <descricao/>
 */
void EnvironmentVariablesService::turnOfConditioner(){

  Serial.println("DESLIGANDO CONDICIONADOR");

  String codigos = __httpRequestService.getComandosIrByIdSalaAndOperacao();

  //------------------------------------------------------    
  String payload = mountPayload("AC", "OFF", codigos);
  sendDataToActuator(TYPE_CONDITIONER, codigos);
  //------------------------------------------------------    

  __monitoring.conditioner = false;
  
  //digitalWrite(LED, LOW);

  __httpRequestService.putMonitoring(__monitoring);
}

/*
 * <descricao> Executa o comando de ligar luzes e envia o status do monitoramento pra o servidor além de gravar a operação em log <descricao/>
 */
void EnvironmentVariablesService::turnOnLight(){

  Serial.println("LIGANDO LUZES");

  __monitoring.light = true;

  // ----------------------------------------------------------
  String payload = mountPayload("LZ", "ON", "null");
  sendDataToActuator(TYPE_LIGHT,"true");  
  // ----------------------------------------------------------

  __httpRequestService.putMonitoring(__monitoring);
}

/*
 * <descricao> Executa o comando de desligar luzes e envia o status do monitoramento pra o servidor além de gravar a operação em log <descricao/>
 */
void EnvironmentVariablesService::turnOfLight(){

  Serial.println("DESLIGANDO LUZES");

  __monitoring.light = false;
  
  // ----------------------------------------------------------
  String payload = mountPayload("LZ", "OFF", "null");
  sendDataToActuator(TYPE_LIGHT, payload);  
  // ----------------------------------------------------------

  __httpRequestService.putMonitoring(__monitoring);
}

void EnvironmentVariablesService::awaitsReturn()
{
  
  unsigned long tempoLimite = millis() + 15000;
  while(millis() <= tempoLimite && !__receivedData)
  { 
      delay(1000);
      //if (configuration.isDebug())
      //{    
        Serial.print("[ClientSocketService] TIME AWAITS: ");
        Serial.println(millis());
      //}
  }    
}

void EnvironmentVariablesService::checkTimeToLoadReservations()
{
  if (__currentTime >= __startTimeLoadReservations && __currentTime <= __endTimeLoadReservations)
  {
       
       if(!__uploadedToday)
       {
           __reservations = __httpRequestService.GetReservationsWeek();

          if(!__uploadedToday)
            __reservations.clear();
       }

  } else 
      __uploadedToday = false; 
}

void EnvironmentVariablesService::CheckEnvironmentVariables()
{
  if (__receivedData) 
  {
    if (__message.equals("S") == 0)
      __hasMovement = true;

    __message = "";
    __receivedData = false;
  }
}

void EnvironmentVariablesService::continuousValidation()
{
  while(true)
  {
      __currentTime = __ntp.getFormattedTime();

      checkTimeToLoadReservations();

      turnOffManagedDevices();
      
      turnOnConditioner();

      delay(2000);
  }
}


String EnvironmentVariablesService::mountPayload(String deviceType, String state, String command)
{
    String payload;
    payload.concat("{");
    payload.concat("\"device_type\":" + deviceType + ", ");
    payload.concat("\"state\":\"" + state + "\", ");
    payload.concat("\"command\":\"" + command + "\", ");
    payload.concat("}");

    return payload;
}
