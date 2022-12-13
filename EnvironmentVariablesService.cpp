#include "EnvironmentVariablesService.h"
#include "Global.h"

String __currentTime;
vector<struct Monitoramento> EnvironmentVariablesService::__monitoringConditioner;
vector<struct Monitoramento> EnvironmentVariablesService::__monitoringLight;
vector<struct Reserva> EnvironmentVariablesService::__reservations; 
HardwareRecord EnvironmentVariablesService::__hardware; 
String __startTimeLoadReservations;
String __endTimeLoadReservations;
bool EnvironmentVariablesService::__hasMovement;
unsigned long EnvironmentVariablesService::__lastTimeAttended;
unsigned long EnvironmentVariablesService::__lastTimeLoadReservations;
BLEServerService* __bleServerConfig;
HTTPService __httpRequestService;
WiFiService __wifiService;
UtilsService __utilsService;
Config __config;

EnvironmentVariablesService::EnvironmentVariablesService()
{
    __startTimeLoadReservations  = "00:05:00";
    __endTimeLoadReservations    = "00:10:00";
    __hasMovement = false;
    __inClass = false;
    __lastTimeAttended = millis();
}

void EnvironmentVariablesService::initEnvironmentVariables() 
{
    __monitoringConditioner = __httpRequestService.getMonitoringByIdSalaAndEquipamento("CONDICIONADOR");
    __monitoringLight = __httpRequestService.getMonitoringByIdSalaAndEquipamento("LUZES");
    __reservations = __httpRequestService.GetReservationsWeek();
    __lastTimeLoadReservations = millis();
    __lastTimeAttended = millis();
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

unsigned long EnvironmentVariablesService::getLastTimeLoadReservations()
{
  return __lastTimeLoadReservations;
}

void EnvironmentVariablesService::setLastTimeLoadReservations(unsigned long time)
{
  __lastTimeLoadReservations = time;
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
  }

  __bleServerConfig->disconnectToActuator();
   
  delay(2000);

  __utilsService.updateMonitoring(ENV_MESSAGE, uuid);

  ENV_RECEIVED_DATA = false;
  ENV_MESSAGE = ""; 
}

void EnvironmentVariablesService::sendDataToActuator(int typeEquipment, String uuid, String message)
{
  if(!__bleServerConfig->isSensorListed(uuid, TYPE_ACTUATOR))
  {
    Serial.println("==================================");         
    Serial.println("[ENVIRONMENT_VARIABLES]: No matching actuator with this uuid: " + uuid);

    return;
  }

  __config.lock();

  ENV_REQUEST = true;

  delay(1000);
  
  for(struct HardwareRecord r : __bleServerConfig->getActuators())
  {
    if(r.uuid.equals(uuid) && typeEquipment == r.typeEquipment)
      sendDataToActuator(r.uuid, message);
  }
  ENV_REQUEST = false;

  __config.unlock();
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
    
    if (__currentTime >= horaInicio && __currentTime < horaFim)
      inClass = true;
  }

  return inClass;
}

/*
 * <descricao> Verifica se é para ligar os dispostivos (luzes e ar) de acordo com as 
 * infomacoes obtidas dos modulos de sensoriamento e dos dados das reservas da sala <descricao/>
 */
void EnvironmentVariablesService::turnOnManagedDevices() {
    
    if (IN_CLASS && __hasMovement) 
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

  bool longTimeWithoutMovement = (millis() - __lastTimeAttended) > TIME_TO_TURN_OFF;

  if (!IN_CLASS || (IN_CLASS && longTimeWithoutMovement)) 
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

  __config.lockEnvVariablesMutex();

  Serial.println("==================================");
  Serial.print("[ENVIRONMENT_VARIABLES]: ");
  Serial.println("[ENVIRONMENT_VARIABLES]: LIGANDO CONDICIONADOR");

  if(WiFi.status() != WL_CONNECTED)
    return;

  String codigos = __httpRequestService.getComandosIrByIdSalaAndOperacao(getUuidActuator(TYPE_CONDITIONER));

  String payload = __utilsService.mountPayload("AC", "ON", codigos);
  sendDataToActuator(TYPE_CONDITIONER, uuid, payload);
  //------------------------------------------------------

  __config.unlockEnvVariablesMutex();

}

/*
 * <descricao> Executa o comando de desligar luzes e envia o status do monitoramento pra o servidor além de gravar a operação em log <descricao/>
 */
void EnvironmentVariablesService::turnOffConditioner(String uuid) {
  
  __config.lockEnvVariablesMutex();

  Serial.println("==================================");
  Serial.print("[ENVIRONMENT_VARIABLES]: ");
  Serial.println("[ENVIRONMENT_VARIABLES]: DESLIGANDO CONDICIONADOR");

  if(WiFi.status() != WL_CONNECTED)
    return;
    
  String codigos = __httpRequestService.getComandosIrByUuidAndOperacao(uuid, TURN_OFF);

  //------------------------------------------------------    
  String payload = __utilsService.mountPayload("AC", "OFF", codigos);
  sendDataToActuator(TYPE_CONDITIONER, uuid, payload);
  //------------------------------------------------------    

  __config.unlockEnvVariablesMutex();

}

/*
 * <descricao> Executa o comando de ligar luzes e envia o status do monitoramento pra o servidor além de gravar a operação em log <descricao/>
 */
void EnvironmentVariablesService::turnOnLight(String uuid){
  
  __config.lockEnvVariablesMutex();

  Serial.println("==================================");
  Serial.print("[ENVIRONMENT_VARIABLES]: ");
  Serial.println("[ENVIRONMENT_VARIABLES]: LIGANDO LUZES");

  // ----------------------------------------------------------
  String payload = __utilsService.mountPayload("LZ", "ON", "null");
  sendDataToActuator(TYPE_LIGHT, uuid, payload);  
  // ----------------------------------------------------------

  __config.unlockEnvVariablesMutex();
}

/*
 * <descricao> Executa o comando de desligar luzes e envia o status do monitoramento pra o servidor além de gravar a operação em log <descricao/>
 */
void EnvironmentVariablesService::turnOffLight(String uuid){

  __config.lockEnvVariablesMutex();

  Serial.println("==================================");
  Serial.print("[ENVIRONMENT_VARIABLES]: ");
  Serial.println("[ENVIRONMENT_VARIABLES]: DESLIGANDO LUZES");
  
  // ----------------------------------------------------------
  String payload = __utilsService.mountPayload("LZ", "OFF", "null");
  sendDataToActuator(TYPE_LIGHT, uuid, payload);  
  // ----------------------------------------------------------

  __config.unlockEnvVariablesMutex();
}

void EnvironmentVariablesService::awaitsReturn()
{
  unsigned long tempoLimite = millis() + TIME_TO_AWAIT_RETURN;
  while(millis() <= tempoLimite && !ENV_RECEIVED_DATA) {}    
}

void EnvironmentVariablesService::checkTimeToLoadReservations()
{
  if(WiFi.status() != WL_CONNECTED)   
    return;

  String currentTime = __httpRequestService.getTime(GET_TIME);

  if(!currentTime.equals(""))
    __currentTime = currentTime;
  
  bool timeToLoadReservations = (millis() - __lastTimeLoadReservations) >= TIME_TO_LOAD;

  if (timeToLoadReservations)
  {
    __reservations = __httpRequestService.GetReservationsWeek();
    setLastTimeLoadReservations(millis());
  } 
}

void EnvironmentVariablesService::checkEnvironmentVariables()
{
  if (ENV_RECEIVED_DATA) 
  {
    struct MonitoringRecord variables = deserealizeData(ENV_MESSAGE);

    if (variables.hasPresent == "S") 
    {
      __hasMovement = true;
      __lastTimeAttended = millis();
    } 
    else 
    {
      __hasMovement = false;
    }

    ENV_MESSAGE = "";
    ENV_RECEIVED_DATA = false; 
  }
}

struct MonitoringRecord EnvironmentVariablesService::deserealizeData(String message)
{
  struct MonitoringRecord environmentVariables = {"", 0.0};
  
  DynamicJsonDocument doc(512);
  DeserializationError error = deserializeJson(doc, message);
  
  if (!error)
  {
    environmentVariables.temperature = doc["temperature"].as<int>();
    environmentVariables.hasPresent = doc["hasPresent"].as<String>();
  }
  else if(__config.isDebug())
  {
    Serial.println("==================================");
    Serial.println("[ENVIRONMENT_VARIABLES] Falha no parse JSON.......");
    Serial.println(error.f_str());
  }

  return environmentVariables;
}

void EnvironmentVariablesService::continuousValidation()
{
  if(__config.isDebug())
  {
    Serial.println("==================================");
    Serial.print("[ENVIRONMENT_VARIABLES]: ");
    Serial.println(__currentTime);
  }
  
  checkTimeToLoadReservations();

  IN_CLASS = getRoomDuringClassTime();

  checkEnvironmentVariables();

  turnOffManagedDevices();
      
  turnOnManagedDevices();

  vTaskDelay(pdMS_TO_TICKS(10000));
}
