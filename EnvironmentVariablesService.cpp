#include "EnvironmentVariablesService.h"

#include <NTPClient.h>
#include <WiFiUDP.h>

String __currentTime;
struct Monitoramento __monitoringConditioner;
struct Monitoramento __monitoringLight;
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
}

void EnvironmentVariablesService::initEnvironmentVariables() 
{
    __ntp.begin();
    __ntp.forceUpdate();
    __monitoringConditioner = __httpRequestService.getMonitoringByIdSalaAndEquipamento("CONDICIONADOR");
    __monitoringLight = __httpRequestService.getMonitoringByIdSalaAndEquipamento("LUZES");
    __reservations = __httpRequestService.GetReservationsWeek();
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

struct Monitoramento EnvironmentVariablesService::getMonitoringLight()
{
  return __monitoringLight;
}

void EnvironmentVariablesService::setMonitoringLight(struct Monitoramento monitoring)
{
  __monitoringLight = monitoring;
}

struct Monitoramento EnvironmentVariablesService::getMonitoringConditioner()
{
  return __monitoringConditioner;
}

void EnvironmentVariablesService::setMonitoringConditioner(struct Monitoramento monitoring)
{
  __monitoringConditioner = monitoring;
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

      if (!__monitoringConditioner.estado)
        turnOnConditioner();

      if (!__monitoringLight.estado)
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
    if (__monitoringConditioner.estado) 
      turnOfConditioner();

    if (__monitoringLight.estado)
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

  __monitoringConditioner.estado = true;

  __httpRequestService.putMonitoring(__monitoringConditioner);
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

  __monitoringConditioner.estado = false;
  
  //digitalWrite(LED, LOW);

  __httpRequestService.putMonitoring(__monitoringConditioner);
}

/*
 * <descricao> Executa o comando de ligar luzes e envia o status do monitoramento pra o servidor além de gravar a operação em log <descricao/>
 */
void EnvironmentVariablesService::turnOnLight(){

  Serial.println("LIGANDO LUZES");

  __monitoringLight.estado = true;

  // ----------------------------------------------------------
  String payload = mountPayload("LZ", "ON", "null");
  sendDataToActuator(TYPE_LIGHT,"true");  
  // ----------------------------------------------------------

  __httpRequestService.putMonitoring(__monitoringLight);
}

/*
 * <descricao> Executa o comando de desligar luzes e envia o status do monitoramento pra o servidor além de gravar a operação em log <descricao/>
 */
void EnvironmentVariablesService::turnOfLight(){

  Serial.println("DESLIGANDO LUZES");

  __monitoringLight.estado = false;
  
  // ----------------------------------------------------------
  String payload = mountPayload("LZ", "OFF", "null");
  sendDataToActuator(TYPE_LIGHT, payload);  
  // ----------------------------------------------------------

  __httpRequestService.putMonitoring(__monitoringLight);
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
  __currentTime = __ntp.getFormattedTime();

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
      checkTimeToLoadReservations();

      CheckEnvironmentVariables();

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
