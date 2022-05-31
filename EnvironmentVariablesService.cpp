#include "EnvironmentVariablesService.h"

String __currentTime;
struct Monitoramento __monitoring;
vector<struct Reserva> EnvironmentVariablesService::__reservations; 
HardwareRecord EnvironmentVariablesService::__hardware; 
String EnvironmentVariablesService::__startTimeLoadReservations;
String EnvironmentVariablesService::__endTimeLoadReservations;
bool EnvironmentVariablesService::__uploadedToday;
NTPClient EnvironmentVariablesService::__ntp;
WiFiUDP EnvironmentVariablesService::__udp;

HTTPService _httpService;
BLEServerService* __bleConfiguration; 

void EnvironmentVariablesService::EnvironmentVariablesService()
{
    __startTimeLoadReservations  = "00:05:00";
    __endTimeLoadReservations    = "00:10:00";
    __uploadedToday = false;
    __ntp = new NTPClient(__udp, "a.st1.ntp.br", -3 * 3600, 60000);
    __ntp.begin();
    __ntp.forceUpdate();
}

std::vector<struct Reserva> EnvironmentVariablesService::getReservations()
{
    return __reservations;
}

void EnvironmentVariablesService::setReservations(std::vector<struct Reserva> reservations)
{
    __reservations = reservations;
}

HardwareRecord EnvironmentVariablesService::getHardware()
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

struct EnvironmentVariablesService::getMonitoring()
{
  return __monitoring;
}

void EnvironmentVariablesService::setMonitoring(struct Monitoramento monitoring)
{
  __monitoring = monitoring;
}

/*
 * <descricao> Verifica se é para ligar os dispostivos (luzes e ar) de acordo com as 
 * infomacoes obtidas dos modulos de sensoriamento e dos dados das reservas da sala <descricao/>
 */
void turnOnManagedDevices() {
  String horaInicio, horaFim, logMonitoramento;
  
  struct Reserva r;
  for (r: __reservations) {

    horaInicio = r.horarioInicio;
    horaFim = r.horarioFim;
    
    if (__currentTime >= r.horarioInicio && __currentTime < r.horarioFim && temGente) {

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
      turnOfConditioner(true);

    if (__monitoring.light)
      turnOfLight(true);
  }
}

/*
 * <descricao> Executa o comando de ligar luzes e envia o status do monitoramento pra o servidor além de gravar a operação em log <descricao/>
 */
void EnvironmentVariablesService::turnOnConditioner(){

  Serial.println("LIGANDO CONDICIONADOR");

  String codigos = _httpService.getComandosIrByIdSalaAndOperacao();
      
  sendDataToActuator(TYPE_CONDITIONER, codigos);

  __monitoring.conditioner = true;

  _httpService.putMonitoring(__monitoring);
}

/*
 * <descricao> Executa o comando de desligar luzes e envia o status do monitoramento pra o servidor além de gravar a operação em log <descricao/>
 */
void EnvironmentVariablesService::turnOfConditioner(){

  Serial.println("DESLIGANDO CONDICIONADOR");

  String codigos = _httpService.getComandosIrByIdSalaAndOperacao();

  senDataToActuator(TYPE_CONDITIONER, codigos);
    
  __monitoring.conditioner = false;
  
  digitalWrite(LED, LOW);

  _httpService.putMonitoring(__monitoring);
}

/*
 * <descricao> Executa o comando de ligar luzes e envia o status do monitoramento pra o servidor além de gravar a operação em log <descricao/>
 */
void EnvironmentVariablesService::turnOnLight(){

  Serial.println("LIGANDO LUZES");

  __monitoring.light = true;

  // ----------------------------------------------------------
  senDataToActuator(TYPE_LIGHT,"true");  
  // ----------------------------------------------------------

  _httpService.putMonitoring(__monitoring);
}

/*
 * <descricao> Executa o comando de desligar luzes e envia o status do monitoramento pra o servidor além de gravar a operação em log <descricao/>
 */
void EnvironmentVariablesService::turnOfLight(){

  Serial.println("DESLIGANDO LUZES");

  __monitoring.light = false;
  
  // ----------------------------------------------------------
  sendDataToActuator(TYPE_LIGHT, "false");  
  // ----------------------------------------------------------

  _httpService.putMonitoring(__monitoring);
}


void EnvironmentVariablesService::sendDataToActuator(String uuid, String message)
{
  __bleConfiguration->setReceivedRequest(true);

  bool dispConnected = connectToActuator(uuid);
                
  if(dispConnected)
  {
      __bleConfiguration->sendMessageToActuator(message);

      awaitsReturn();

      __bleConfiguration->disconnectToActuator();
  }
                
  __bleConfiguration->setReceivedRequest(false);
} 

void EnvironmentVariablesService::sendDataToActuator(int typeEquipment, String message)
{
  for(struct Actuator a : __bleConfiguration.getActuators())
  {
    if(a.typeEquipment == typeEquipment)
      sendDataToActuator(a.uuid, codigos);
  }
}

void EnvironmentVariablesService::awaitsReturn()
{
  
  unsigned long tempoLimite = millis() + 15000;
  while(millis() <= tempoLimite && !__messageReturned)
  { 
      delay(1000);
      if (configuration.isDebug())
      {    
        Serial.print("[ClientSocketService] TIME AWAITS: ");
        Serial.println(millis());
      }
  }    
}

void EnvironmentVariablesService::checkTimeToLoadReservations(){
  if (__currentTime >= __startTimeLoadReservations && __currentTime <= __endTimeLoadReservations){
       
       if(!__uploadedToday){
           __reservations = _httpService.GetReservationsWeek;

          if(!__uploadedToday)
            __reservations.clear();
       }

  } else 
      __uploadedToday = false; 
}

void EnvironmentVariablesService::continuousValidation()
{
  while(true)
  {
      __currentTime = __ntp.getFormattedTime();

      turnOffManagedDevices();
      turnOnConditioner();

      checkTimeToLoadReservations();
  }
}