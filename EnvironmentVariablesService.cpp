#include "EnvironmentVariablesService.h"

String __currentTime;
struct Monitoramento __monitoring;
vector<struct Reserva> EnvironmentVariablesService::__reservations; 
HardwareRecord EnvironmentVariablesService::__hardware; 
HTTPService _httpService;
BLEServerService* __bleConfiguration; 

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
void ligarDispositivosGerenciaveis() {
  String horaInicio, horaFim, logMonitoramento;
  
  struct Reserva r;
  for (r: __reservations) {

    horaInicio = r.horarioInicio;
    horaFim = r.horarioFim;
    
    if (__currentTime >= r.horarioInicio && __currentTime < r.horarioFim && temGente) {

      if (!__monitoring.conditioner) {

        vector < int > listaCodigos = obterComandosIrByIdSalaAndOperacao(operacao_ligar);
        
        //enviarComandosIr(listaCodigos);
        //enviarMonitoramento(lightStatus, __monitoring.conditioner);

        __monitoring.conditioner = true;
        //digitalWrite(LED, HIGH);

        Serial.println("Ligando ar condicionado");
        Serial.print("Hora: ");
        Serial.println(__currentTime);

      }

      if (!__monitoring.light) {

        /*
         * Ligando luzes
         */
         ligarLuzes(true);
      }
    }
  }
}

/*
 * <descricao> Verifica se é para desligar os dispostivos (luzes e ar) de acordo com as 
 * informacoes obtidas dos modulos de sensoriamento e dos dados das reservas da sala <descricao/>
 */
void EnvironmentVariablesService::desligarDispositivosGerenciaveis() {
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
    if (__monitoring.conditioner) {

      vector<int> listaCodigos = obterComandosIrByIdSalaAndOperacao(operacao_desligar);
      //enviarComandosIr(listaCodigos);

      //enviarMonitoramento(lightStatus, conditionerStatus);

      Serial.println("Desligando ar condicionado");
      Serial.print("Hora: ");
      Serial.println(__currentTime);

      __monitoring.conditioner = false;
      //digitalWrite(LED, LOW);

    }

    if (__monitoring.light) {

      desligarLuzes(true);

    }
  }
}

/*
 * <descricao> Executa o comando de ligar luzes e envia o status do monitoramento pra o servidor além de gravar a operação em log <descricao/>
 */
void EnvironmentVariablesService::ligarLuzes(bool enviarDadosMonitoramento){

  Serial.println("LIGANDO");

  __monitoring.light = true;

  // ----------------------------------------------------------
  senDataToActuator(__monitoring);  
  // ----------------------------------------------------------

  _httpService.putMonitoring(__monitoring);
}

/*
 * <descricao> Executa o comando de desligar luzes e envia o status do monitoramento pra o servidor além de gravar a operação em log <descricao/>
 */
void EnvironmentVariablesService::desligarLuzes(bool enviarDadosMonitoramento){

  Serial.println("DESLIGANDO");

  __monitoring.light = false;
  
  // ----------------------------------------------------------
  senDataToActuator(__monitoring);  
  // ----------------------------------------------------------

  _httpService.putMonitoring(__monitoring);
}


bool EnvironmentVariablesService::senDataToActuator(struct Monitoramento monitoring)
{
  __bleConfiguration->setReceivedRequest(true);

  bool dispConnected = connectToActuator(request.uuid);
                
  if(dispConnected)
  {
      __bleConfiguration->sendMessageToActuator(request.code);

      awaitsReturn();

      __bleConfiguration->disconnectToActuator();
  }
                
  __bleConfiguration->setReceivedRequest(false);
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
