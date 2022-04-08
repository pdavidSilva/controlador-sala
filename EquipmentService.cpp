#include "EquipmentService.h"

bool __lightOn;
EnergyMonitor __sct13;

EquipmentService::EquipmentService() {}

bool EquipmentService::getLightOn(){
    return __lightOn;
}
  
void EquipmentService::setLightOn(bool light){
  __lightOn = light;
}

EnergyMonitor EquipmentService::geSct(){
  return __sct13;
}

void EquipmentService::setSct(EnergyMonitor sct){
  __sct13 = sct;
}

/*
 * <descricao> Obtem nome do dispositivo ou os codigos IR neviados na requisicao do servidor  <descricao/>
 * <parametros> data: codigos IR recebidos na requisicao do servidor <parametros/>
 * <parametros> separator: caracter chave para realizar o 'split' <parametros/>
 * <parametros> index: identificar que diz se quem chama quer receber o nome do dispositivo ou os codigos IR <parametros/>
 * <retorno> string com nome do dispotivo recebido na requisicao ou os codigos IR <retorno/>
 */
String EquipmentService::SplitGetIndex(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {
    0,
    -1
  };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}


/*
 * <descricao>   <descricao/>
 * <parametros> data: codigos IR recebidos na requisicao do servidor <parametros/>
 * <retorno>  <retorno/>
 */
Vector<int> EquipmentService::SplitIrComands(String data) {
    
    int storage_array[200]; // uso do vetor tem que declarar um valor max
    Vector <int> codigo;
    codigo.setStorage(storage_array);

    String codigoString = SplitGetIndex(data, ';', 1);
	
    String temp = "";
    for (int i = 0; i < codigoString.length(); i++) 
    {
      
      if (codigoString.charAt(i) == ',' || i == codigoString.length() - 1) 
      {
        codigo.push_back(temp.toInt());
        temp = "";
      } 
      else 
      {
        if (codigoString.charAt(i) != ';' || codigoString.charAt(i) != ' ')
          temp += codigoString.charAt(i);
      }

    }


    return codigo;
}


/*
 * <descricao>   <descricao/>
 * <parametros> data: codigos IR recebidos na requisicao do servidor <parametros/>
 * <retorno>  <retorno/>
 */
void EquipmentService::SendIrComand(Vector<int> codigo) {
    
    int k = 0;
    uint16_t rawData[codigo.size()];
    for (int el: codigo) 
    {
      rawData[k] = (uint16_t) el;
      k++;
    }
    
    
    irsend.sendRaw(rawData, codigo.size(), 38); // envia comando IR para o equipamento    
    delay(1000);
}

/*
 * <descricao>   <descricao/>
 * <parametros> data: codigos IR recebidos na requisicao do servidor <parametros/>
 * <retorno>  <retorno/>
 */
bool EquipmentService::checkIrms() {
    
    double Irms = SCT013.calcIrms(1480); // Calcula o valor da Corrente
    int potencia = Irms * tensao; // Calcula o valor da Potencia Instantanea  
    
    if (Irms > 2) // se a corrente for maior que (valor de Ampere considerado ligado, é enviado a resposta para aplicação que o sensor está ligado
       return true;
    
    return false; 
}


/*
 * <descricao> Executa o comando de ligar luzes e envia o status do monitoramento pra o servidor além de gravar a operação em log <descricao/>
 */
void EquipmentService::checkOperationLights(String msg){
    
    String operationTurnOnOff = SplitGetIndex(msg, ';', 1);
    
    if(operationTurnOnOff == "True;")
      turnOnLights();
    else
      turnOffLights();
}

/*
 * <descricao> Executa o comando de ligar luzes e envia o status do monitoramento pra o servidor além de gravar a operação em log <descricao/>
 */
void EquipmentService::turnOnLights(){
  /*
   * Ligando luzes
   */
  Serial.println("LIGANDO");

  __lightOn = true;
  digitalWrite(RELE, HIGH);

  //String logMonitoramento = "Ligando luzes no horario: " + horaAtualSistema;
  //gravarLinhaEmArquivo(SPIFFS, logMonitoramento, pathLogMonitoramento);
}

/*
 * <descricao> Executa o comando de desligar luzes e envia o status do monitoramento pra o servidor além de gravar a operação em log <descricao/>
 */
void EquipmentService::turnOffLights(){
  /*
   * Desligando luzes
   */
  Serial.println("DESLIGANDO");

  __lightOn = false;
  digitalWrite(RELE, LOW);

  //String logMonitoramento = "Desligando luzes no horario: " + horaAtualSistema;
  //gravarLinhaEmArquivo(SPIFFS, logMonitoramento, pathLogMonitoramento);
}