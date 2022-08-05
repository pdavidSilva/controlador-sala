#include "EnvironmentVariablesService.h"

#include <NTPClient.h>
#include <WiFiUDP.h>

String __currentTime;
struct Monitoramento EnvironmentVariablesService::__monitoringConditioner;
struct Monitoramento EnvironmentVariablesService::__monitoringLight;
vector<struct Reserva> EnvironmentVariablesService::__reservations; 
HardwareRecord EnvironmentVariablesService::__hardware; 
String __startTimeLoadReservations;
String __endTimeLoadReservations;
bool EnvironmentVariablesService::__uploadedToday;
bool EnvironmentVariablesService::__receivedData;
bool EnvironmentVariablesService::__hasMovement;
bool EnvironmentVariablesService::__inClass;
String EnvironmentVariablesService::__message;
WiFiUDP __udp;
NTPClient __ntp(__udp, "a.st1.ntp.br", -3 * 3600, 60000);

BLEServerService* __bleServerConfig;
HTTPService __httpRequestService;
WiFiService __wifiService;
UtilsService __utilsService;

EnvironmentVariablesService::EnvironmentVariablesService()
{
    __startTimeLoadReservations  = "00:05:00";
    __endTimeLoadReservations    = "00:10:00";
    __uploadedToday = false;
    __hasMovement = false;
    __inClass = false;
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

bool EnvironmentVariablesService::setInClass(bool inClass)
{
    __inClass = inClass;
}

bool EnvironmentVariablesService::getInClass()
{
    return __inClass;
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
  Serial.println("==================================");
  Serial.println("[ENVIRONMENT_VARIABLES]: CONECTANDO AO ATUADOR");
  Serial.print("[ENVIRONMENT_VARIABLES]: ");
  Serial.println(uuid);
  Serial.print("[ENVIRONMENT_VARIABLES]: ");
  Serial.println(message);

  bool dispConnected = __bleServerConfig->connectToActuator(uuid);
                
  if(dispConnected)
  {
      __bleServerConfig->sendMessageToActuator(message);

      awaitsReturn();

      __bleServerConfig->disconnectToActuator();
  }

  __utilsService.updateMonitoring(__message);

  __receivedData = false;
  __message = ""; 
}

void EnvironmentVariablesService::sendDataToActuator(int typeEquipment, String message)
{
  __bleServerConfig->setReceivedRequest(true);
  __bleServerConfig->setEnvironmentSolicitation(true);

  delay(1000);
  
  for(struct HardwareRecord r : __bleServerConfig->getActuators())
  {
    if(r.typeEquipment == typeEquipment)
      sendDataToActuator(r.uuid, message);
  }

  __bleServerConfig->setReceivedRequest(false);
  __bleServerConfig->setEnvironmentSolicitation(false);
}

String EnvironmentVariablesService::getUuidActuator(int typeEquipment)
{
  String uuid = "";
  for(struct HardwareRecord r : __bleServerConfig->getActuators())
  {
    if(r.typeEquipment == typeEquipment)
      uuid = r.uuid;
  }

  return uuid;
}

/*
 * <descricao> Verifica se é para ligar os dispostivos (luzes e ar) de acordo com as 
 * infomacoes obtidas dos modulos de sensoriamento e dos dados das reservas da sala <descricao/>
 */
void EnvironmentVariablesService::turnOnManagedDevices() {
  String horaInicio, horaFim;
  bool inClass = false;
  struct Reserva r;
  for (r: __reservations) {

    horaInicio = r.horarioInicio;
    horaFim = r.horarioFim;
    
    if (__currentTime >= r.horarioInicio && __currentTime < r.horarioFim && __hasMovement) {

      if (!__monitoringConditioner.estado)
        turnOnConditioner();

      if (!__monitoringLight.estado)
        turnOnLight();

        inClass = true;
    }
  }

  __inClass = inClass;
  __hasMovement = false;
}

/*
 * <descricao> Verifica se é para desligar os dispostivos (luzes e ar) de acordo com as 
 * informacoes obtidas dos modulos de sensoriamento e dos dados das reservas da sala <descricao/>
 */
void EnvironmentVariablesService::turnOffManagedDevices() {
  String horaInicio;
  String horaFim;
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

  __inClass = !notInClass;
}

/*
 * <descricao> Executa o comando de ligar luzes e envia o status do monitoramento pra o servidor além de gravar a operação em log <descricao/>
 */
void EnvironmentVariablesService::turnOnConditioner(){

  Serial.println("==================================");
  Serial.print("[ENVIRONMENT_VARIABLES]: ");
  Serial.println(__monitoringConditioner.estado ? "true" : "false");
  Serial.println("[ENVIRONMENT_VARIABLES]: LIGANDO CONDICIONADOR");

  String codigos = __httpRequestService.getComandosIrByIdSalaAndOperacao(getUuidActuator(TYPE_CONDITIONER));

  //------------------------------------------------------    
  String payload = __utilsService.mountPayload("AC", "ON", codigos);
  sendDataToActuator(TYPE_CONDITIONER, payload);
  //------------------------------------------------------

  __monitoringConditioner.estado = true;

  if(__monitoringConditioner.equipamentoId > 0 && __monitoringConditioner.id > 0)
    __httpRequestService.putMonitoring(__monitoringConditioner);
}

/*
 * <descricao> Executa o comando de desligar luzes e envia o status do monitoramento pra o servidor além de gravar a operação em log <descricao/>
 */
void EnvironmentVariablesService::turnOfConditioner(){
  
  Serial.println("==================================");
  Serial.print("[ENVIRONMENT_VARIABLES]: ");
  Serial.println(__monitoringConditioner.estado ? "true" : "false");
  Serial.println("[ENVIRONMENT_VARIABLES]: DESLIGANDO CONDICIONADOR");

  String codigos = __httpRequestService.getComandosIrByIdSalaAndOperacao(getUuidActuator(TYPE_CONDITIONER));

  //------------------------------------------------------    
  String payload = __utilsService.mountPayload("AC", "OFF", codigos);
  sendDataToActuator(TYPE_CONDITIONER, codigos);
  //------------------------------------------------------    

  __monitoringConditioner.estado = false;
  
  if(__monitoringConditioner.equipamentoId > 0 && __monitoringConditioner.id > 0)
    __httpRequestService.putMonitoring(__monitoringConditioner);
}

/*
 * <descricao> Executa o comando de ligar luzes e envia o status do monitoramento pra o servidor além de gravar a operação em log <descricao/>
 */
void EnvironmentVariablesService::turnOnLight(){

  Serial.println("==================================");
  Serial.print("[ENVIRONMENT_VARIABLES]: ");
  Serial.println(__monitoringLight.estado ? "true" : "false");
  Serial.println("[ENVIRONMENT_VARIABLES]: LIGANDO LUZES");

  __monitoringLight.estado = true;

  // ----------------------------------------------------------
  String payload = __utilsService.mountPayload("LZ", "ON", "null");
  sendDataToActuator(TYPE_LIGHT,"true");  
  // ----------------------------------------------------------

  if(__monitoringLight.equipamentoId > 0 && __monitoringLight.id > 0)
    __httpRequestService.putMonitoring(__monitoringLight);
}

/*
 * <descricao> Executa o comando de desligar luzes e envia o status do monitoramento pra o servidor além de gravar a operação em log <descricao/>
 */
void EnvironmentVariablesService::turnOfLight(){

  Serial.println("==================================");
  Serial.print("[ENVIRONMENT_VARIABLES]: ");
  Serial.println(__monitoringLight.estado ? "true" : "false");
  Serial.println("[ENVIRONMENT_VARIABLES]: DESLIGANDO LUZES");

  __monitoringLight.estado = false;
  
  // ----------------------------------------------------------
  String payload = __utilsService.mountPayload("LZ", "OFF", "null");
  sendDataToActuator(TYPE_LIGHT, payload);  
  // ----------------------------------------------------------

  if(__monitoringLight.equipamentoId > 0 && __monitoringLight.id > 0)
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
      //  Serial.print("[ENVIRONMENT_VARIABLES]: TIME AWAITS: ");
      //  Serial.println(millis());
      //}
  }    
}

void EnvironmentVariablesService::checkTimeToLoadReservations()
{
  __wifiService.connect();
    
  __currentTime = __ntp.getFormattedTime();

  __wifiService.disconnect();

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

void EnvironmentVariablesService::checkEnvironmentVariables()
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
  Config config;
  int checkTimeToLoad = 0;
  checkTimeToLoadReservations();

  while(true)
  {    
      if(config.isDebug())
      {
        Serial.println("==================================");
        Serial.print("[ENVIRONMENT_VARIABLES]: ");
        Serial.println(__currentTime);
      }
      
      checkEnvironmentVariables();

      turnOffManagedDevices();
      
      turnOnManagedDevices();

      if(checkTimeToLoad == CHECK_TIME_TO_LOAD)
      {
        checkTimeToLoadReservations();
        checkTimeToLoad = 0;
      }
      
      checkTimeToLoad++;

      delay(1000);
  }
}
