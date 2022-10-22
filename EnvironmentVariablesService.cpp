#include "EnvironmentVariablesService.h"

String __currentTime;
vector<struct Monitoramento> EnvironmentVariablesService::__monitoringConditioner;
vector<struct Monitoramento> EnvironmentVariablesService::__monitoringLight;
vector<struct Reserva> EnvironmentVariablesService::__reservations; 
HardwareRecord EnvironmentVariablesService::__hardware; 
String __startTimeLoadReservations;
String __endTimeLoadReservations;
bool EnvironmentVariablesService::__uploadedToday;
bool EnvironmentVariablesService::__receivedData;
bool EnvironmentVariablesService::__hasMovement;
bool EnvironmentVariablesService::__inClass;
String EnvironmentVariablesService::__message;
unsigned long EnvironmentVariablesService::__lastTimeAttended;
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
    __lastTimeAttended = millis();
}

void EnvironmentVariablesService::initEnvironmentVariables() 
{
    __ntp.begin();
    __ntp.forceUpdate();
    __monitoringConditioner = __httpRequestService.getMonitoringByIdSalaAndEquipamento("CONDICIONADOR");
    __monitoringLight = __httpRequestService.getMonitoringByIdSalaAndEquipamento("LUZES");
    __reservations = __httpRequestService.GetReservationsWeek();
}

bool EnvironmentVariablesService::hasConditionerTurnOn() 
{
  struct Monitoramento monitoring = {0, false, "", 0};
  for(monitoring : __monitoringConditioner) {
    if (monitoring.estado && monitoring.id > 0 && monitoring.equipamentoId > 0)
      return true;
  }

  return false;
}

bool EnvironmentVariablesService::hasLightTurnOn() 
{
  struct Monitoramento monitoring = {0, false, "", 0};
  for(monitoring : __monitoringLight) {
    if (monitoring.estado && monitoring.id > 0 && monitoring.equipamentoId > 0)
      return true;
  }

  return false;
}

bool EnvironmentVariablesService::hasConditionerTurnOff() 
{
  struct Monitoramento monitoring = {0, false, "", 0};
  for(monitoring : __monitoringConditioner) {
    if (!monitoring.estado && monitoring.id > 0 && monitoring.equipamentoId > 0)
      return true;
  }

  return false;
}

bool EnvironmentVariablesService::hasLightTurnOff() 
{
  struct Monitoramento monitoring = {0, false, "", 0};
  for(monitoring : __monitoringLight) {
    if (!monitoring.estado && monitoring.id > 0 && monitoring.equipamentoId > 0)
      return true;
  }

  return false;
}

unsigned long EnvironmentVariablesService::getLastTimeAttended() 
{
    return __lastTimeAttended;
}

void EnvironmentVariablesService::setLastTimeAttended(unsigned long time) 
{
    __lastTimeAttended = time;
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

struct Monitoramento EnvironmentVariablesService::getMonitoringLightByUuid(String uuid)
{
  struct Monitoramento monitoring = {0, false, "", 0};

  for(monitoring : __monitoringLight) {
    if (monitoring.uuid.equals(uuid))
      return monitoring;
  }

  return {0, false, "", 0};
}

std::vector<struct Monitoramento> EnvironmentVariablesService::getMonitoringLight()
{
  return __monitoringLight;
}

void EnvironmentVariablesService::setMonitoringLight(std::vector<struct Monitoramento> monitoring)
{
  __monitoringLight = monitoring;
}

void EnvironmentVariablesService::updateMonitoringLight(struct Monitoramento monitoring)
{ 
  for(int i = 0; i < __monitoringLight.size();i++) {
    if (__monitoringLight.at(i).id == monitoring.id)
      __monitoringLight.at(i).estado = monitoring.estado;
  }
}

struct Monitoramento EnvironmentVariablesService::getMonitoringConditionerByUuid(String uuid)
{
 struct Monitoramento monitoring = {0, false, "", 0};

  for(monitoring : __monitoringConditioner) {
    if (monitoring.uuid.equals(uuid))
      return monitoring;
  }

  return {0, false, "", 0};
}

std::vector<struct Monitoramento> EnvironmentVariablesService::getMonitoringConditioner()
{
  return __monitoringConditioner;
}

void EnvironmentVariablesService::setMonitoringConditioner(std::vector<struct Monitoramento> monitoring)
{
  __monitoringConditioner = monitoring;
}

void EnvironmentVariablesService::updateMonitoringConditioner(struct Monitoramento monitoring)
{
  for(int i = 0; i < __monitoringConditioner.size(); i++) {
    if (__monitoringConditioner.at(i).id == monitoring.id)
      __monitoringConditioner.at(i).estado = monitoring.estado;
  }
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
    std::vector<String> subStrings = __utilsService.splitPayload(message, MAX_LENGTH_PACKET);

    String packet;
    for (packet : subStrings)
    {
      Serial.println("==================================");         
      Serial.println("[ENVIRONMENT_VARIABLES] Sendig packet: " + packet);
      __bleServerConfig->sendMessageToActuator(packet);
    }
        
    awaitsReturn();

    __bleServerConfig->disconnectToActuator();
  }

  __utilsService.updateMonitoring(__message, uuid);

  __receivedData = false;
  __message = ""; 
}

void EnvironmentVariablesService::sendDataToActuator(int typeEquipment, String uuid, String message)
{
  for(struct HardwareRecord r : __bleServerConfig->getActuators())
  {
    if(r.uuid.equals(uuid) && typeEquipment == r.typeEquipment)
      sendDataToActuator(r.uuid, message);
  }
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


std::vector<String> EnvironmentVariablesService::getActuatorsByTypeEquipment(int typeEquipment)
{
  std::vector<String> uuids;
  for(struct HardwareRecord r : __bleServerConfig->getActuators())
  {
    if(r.typeEquipment == typeEquipment)
      uuids.push_back(r.uuid);
  }

  return uuids;
}

/*
 * <descricao> Verifica se a sala está reservada no horário atual e retorna TRUE se estiver em horário de reserva <descricao/>
 */
bool EnvironmentVariablesService::getRoomDuringClassTime() {
  
  String horaInicio, horaFim;
  bool inClass = false;
  struct Reserva r;
  
  for (r: __reservations) {

    horaInicio = r.horarioInicio;
    horaFim = r.horarioFim;
    
    if (__currentTime >= r.horarioInicio && __currentTime < r.horarioFim)
      inClass = true;
  }

  return inClass;
}

/*
 * <descricao> Verifica se é para ligar os dispostivos (luzes e ar) de acordo com as 
 * infomacoes obtidas dos modulos de sensoriamento e dos dados das reservas da sala <descricao/>
 */
void EnvironmentVariablesService::turnOnManagedDevices() {
    
    if (__inClass && __hasMovement) 
    {
      if(hasConditionerTurnOff() || hasLightTurnOff())
      {
          __bleServerConfig->setReceivedRequest(true);
          __bleServerConfig->setEnvironmentSolicitation(true);

          struct Monitoramento monitoring = {0, false, "", 0};

          for(monitoring : __monitoringConditioner) {
            if (!monitoring.estado && monitoring.id > 0 && monitoring.equipamentoId > 0)
              turnOnConditioner(monitoring.uuid);
          }

          for(monitoring : __monitoringLight) {
            if (!monitoring.estado && monitoring.id > 0 && monitoring.equipamentoId > 0)
              turnOnLight(monitoring.uuid);
          }
          
          __bleServerConfig->setReceivedRequest(false);
          __bleServerConfig->setEnvironmentSolicitation(false);
      }
    }  
}

/*
 * <descricao> Verifica se é para desligar os dispostivos (luzes e ar) de acordo com as 
 * informacoes obtidas dos modulos de sensoriamento e dos dados das reservas da sala <descricao/>
 */
void EnvironmentVariablesService::turnOffManagedDevices() {

  bool longTimeWithoutMovement = (millis() - __lastTimeAttended) > CHECK_TIME_TO_TURN_OFF;

  if (!__inClass || (__inClass && longTimeWithoutMovement)) 
  {
    if(hasConditionerTurnOn() || hasLightTurnOn())
    {
      __bleServerConfig->setReceivedRequest(true);
      __bleServerConfig->setEnvironmentSolicitation(true);

      struct Monitoramento monitoring = {0, false, "", 0};

      for(monitoring : __monitoringConditioner) {
        if (monitoring.estado && monitoring.id > 0 && monitoring.equipamentoId > 0) {
          turnOfConditioner(monitoring.uuid);
        }
      }

      for(monitoring : __monitoringLight) {
        if (monitoring.estado && monitoring.id > 0 && monitoring.equipamentoId > 0) {
          turnOfLight(monitoring.uuid);
        }
      }

      __bleServerConfig->setReceivedRequest(false);
      __bleServerConfig->setEnvironmentSolicitation(false);
    }
  }
}

/*
 * <descricao> Executa o comando de ligar luzes e envia o status do monitoramento pra o servidor além de gravar a operação em log <descricao/>
 */
void EnvironmentVariablesService::turnOnConditioner(String uuid) {

  Serial.println("==================================");
  Serial.print("[ENVIRONMENT_VARIABLES]: ");
  Serial.println("[ENVIRONMENT_VARIABLES]: LIGANDO CONDICIONADOR");

  String codigos = __httpRequestService.getComandosIrByUuidAndOperacao(uuid, TURN_ON);

  String payload = __utilsService.mountPayload("AC", "ON", codigos);
    
  sendDataToActuator(TYPE_CONDITIONER, uuid, payload);
}

/*
 * <descricao> Executa o comando de desligar luzes e envia o status do monitoramento pra o servidor além de gravar a operação em log <descricao/>
 */
void EnvironmentVariablesService::turnOffConditioner(String uuid) {
  
  Serial.println("==================================");
  Serial.print("[ENVIRONMENT_VARIABLES]: ");
  Serial.println("[ENVIRONMENT_VARIABLES]: DESLIGANDO CONDICIONADOR");

  String codigos = __httpRequestService.getComandosIrByUuidAndOperacao(uuid, TURN_OFF);

  //------------------------------------------------------    
  String payload = __utilsService.mountPayload("AC", "OFF", codigos);
  sendDataToActuator(TYPE_CONDITIONER, uuid, payload);
  //------------------------------------------------------    
}

/*
 * <descricao> Executa o comando de ligar luzes e envia o status do monitoramento pra o servidor além de gravar a operação em log <descricao/>
 */
void EnvironmentVariablesService::turnOnLight(String uuid){

  Serial.println("==================================");
  Serial.print("[ENVIRONMENT_VARIABLES]: ");
  Serial.println("[ENVIRONMENT_VARIABLES]: LIGANDO LUZES");

  // ----------------------------------------------------------
  String payload = __utilsService.mountPayload("LZ", "ON", "null");
  sendDataToActuator(TYPE_LIGHT, uuid, payload);  
  // ----------------------------------------------------------
}

/*
 * <descricao> Executa o comando de desligar luzes e envia o status do monitoramento pra o servidor além de gravar a operação em log <descricao/>
 */
void EnvironmentVariablesService::turnOffLight(String uuid){

  Serial.println("==================================");
  Serial.print("[ENVIRONMENT_VARIABLES]: ");
  Serial.println("[ENVIRONMENT_VARIABLES]: DESLIGANDO LUZES");
  
  // ----------------------------------------------------------
  String payload = __utilsService.mountPayload("LZ", "OFF", "null");
  sendDataToActuator(TYPE_LIGHT, uuid, payload);  
  // ----------------------------------------------------------

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

String EnvironmentVariablesService::getNow()
{
  return "2022-09-09T09:15:00";
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
    {
      __hasMovement = true;
      __lastTimeAttended = millis();
    } 
    else 
    {
      __hasMovement = false;
    }

    __message = "";
    __receivedData = false; 
  }
}

void EnvironmentVariablesService::continuousValidation()
{
  Config config;
  int checkTimeToLoad = 0;

  while(true)
  {    
      if(config.isDebug())
      {
        Serial.println("==================================");
        Serial.print("[ENVIRONMENT_VARIABLES]: ");
        Serial.println(__currentTime);
      }

      __inClass = getRoomDuringClassTime();
      
      checkEnvironmentVariables();

      turnOffManagedDevices();
      
      turnOnManagedDevices();

      if(checkTimeToLoad == CHECK_TIME_TO_LOAD)
      {
        __reservations = __httpRequestService.GetReservationsWeek();
        checkTimeToLoad = 0;
      }
      
      checkTimeToLoad++;

      delay(1000);
  }
}
