#include "Config.h"
#include "HTTPService.h"

const size_t CAPACITY = JSON_ARRAY_SIZE(12);

HTTPService::HTTPService() {}

void HTTPService::getInfoHardware(HardwareRecord &record)
{

    // hardware/MAC?api_token = TOKEN_BASE&tipo-hardware=TIPO

    Config config;
    Hardware hardware;
    HTTP http;
    String route;

    if (config.getRoute() == 1)
        route = "/hardware/";
    else
        route = "/hardwaredesala/info/";

    String routeService;
    String type = "GET";
    String params = "";
    String token = "594ac3eb82b5080393ad5c426f61c1ed5ac53f90e1abebc15316888cf1c8f5fe";
    String response;

    routeService.concat(route);
    routeService.concat(hardware.getMacAddress());
    routeService.concat("?token=");
    routeService.concat(token);
    routeService.concat("&tipo-hardware=");
    routeService.concat(config.getType());

    response = http.request(routeService, type, params);

    if (strstr(response.c_str(), "[ERROR]") == NULL)
    {
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, response);

        if (error)
        {
            if (config.isDebug())
            {
                Serial.println("==================================");
                Serial.println("[HTTPService] Falha no parse JSON.......");
                Serial.println(error.f_str());
            }
            delay(5000);

            return;
        }

        if (doc["httpCode"].as<int>() == 200)
        {

            record.id = doc["result"]["id"].as<int>();
            record.token = doc["result"]["token"].as<char *>();
            record.uuid = doc["result"]["uuid"].as<char *>();

            return;
        }
        else
        {
            if (config.isDebug())
            {
                Serial.println("==================================");
                Serial.print("[HTTPService] Mensagem: ");
                Serial.println(doc["message"].as<char *>());
            }
            return;
        }
    }
    else
    {
        return;
    }

    return;
}

bool HTTPService::registerHardware(HardwareRecord hardware)
{

    //hardware/register

    Config config;
    HTTP http;
    String route;

    if (config.getRoute() == 1)
        route = "/hardware/register";
    else
        route = "/hardwaredesala/register";

    String routeService;
    String type = "POST";
    String params = "";
    String response;

    String id = String(hardware.id);
    String tipo_hardware_id = String(config.getType()); 

    params.concat("{");
    params.concat("\"id\":" + id + ", ");
    params.concat("\"uuid\":\"" + hardware.uuid + "\", ");
    params.concat("\"token\":\"" + hardware.token + "\", ");
    params.concat("\"tipo_hardware_id\": " + tipo_hardware_id);
    params.concat("}");

    routeService.concat(route);
    response = http.request(routeService, type, params);

    return true;

    // if (strstr(response.c_str(), "[ERROR]") == NULL){
    //     DynamicJsonDocument doc(1024);
    //     DeserializationError error = deserializeJson(doc, response);

    //     if (error) {
    //         if (config.isDebug())
    //         {
    //             Serial.println("==================================");
    //             Serial.println("[HTTPService] Falha no parse JSON.......");
    //             Serial.println(error.f_str());
    //         }
    //         delay(5000);

    //         return false;
    //     }

    //     if(doc["httpCode"].as<int>() == 200){

    //         record.id = doc["result"]["id"].as<int>();
    //         record.token = doc["result"]["token"].as<char*>();
    //         record.uuid = doc["result"]["uuid"].as<char*>();

    //         return false;
    //     }else{
    //         if (config.isDebug())
    //         {
    //             Serial.println("==================================");
    //             Serial.print("[HTTPService] Mensagem: ");
    //             Serial.println(doc["message"].as<char*>());
    //         }
    //         return false;
    //     }

    // }else{
    //     return false;
    // }

    return false;
}

void HTTPService::getSensors(HardwareRecord hardware, String sensors[], int &indexSensors)
{

    //hardware/{uuid}/get-sensors?token=TOKEN

    Config config;
    HTTP http;
    String route;

    if (config.getRoute() == 1)
        route = "/hardware/";
    else
        route = "/hardwaredesala/";

    String routeService;
    String type = "GET";
    String params = "";
    String response;

    String uuid = String(hardware.uuid);
    String token = hardware.token;

    routeService.concat(route);
    routeService.concat(uuid);
    routeService.concat("/get-sensors");
    routeService.concat("?token=");
    routeService.concat(token);

    response = http.request(routeService, type, params);

    if (strstr(response.c_str(), "[ERROR]") == NULL)
    {
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, response);

        if (error)
        {
            if (config.isDebug())
            {
                Serial.println("==================================");
                Serial.println("[HTTPService] Falha no parse JSON.......");
                Serial.println(error.f_str());
            }
            delay(5000);

            return;
        }

        if (doc["httpCode"].as<int>() == 200)
        {

            JsonArray jsonSensors = doc["result"]["sensores"].as<JsonArray>();

            int i = 0;
            for (JsonVariant sensor : jsonSensors)
            {
                sensors[i] = sensor["uuid"].as<char *>();
                i++;
            }

            indexSensors = doc["result"]["length"].as<int>();

            return;
        }
        else
        {
            if (config.isDebug())
            {
                Serial.println("==================================");
                Serial.print("[HTTPService] Mensagem: ");
                Serial.println(doc["message"].as<char *>());
            }
            return;
        }
    }
    else
    {
        return;
    }

    return;
}

void HTTPService::getDevices(HardwareRecord hardware, String devices[], int &indexDevices)
{
}

bool HTTPService::getMaster(HardwareRecord hardware, String &master)
{
    //hardware/{uuid}/get-master?token=TOKEN

    Config config;
    HTTP http;
    String route;

    if (config.getRoute() == 1)
        route = "/hardware/";
    else
        route = "/hardwaredesala/slave/";

    String routeService;
    String type = "GET";
    String params = "";
    String response;

    String uuid = String(hardware.uuid);
    String token = hardware.token;

    routeService.concat(route);
    routeService.concat(uuid);
    routeService.concat("/get-master");
    routeService.concat("?token=");
    routeService.concat(token);

    response = http.request(routeService, type, params);

    if (strstr(response.c_str(), "[ERROR]") == NULL)
    {
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, response);

        if (error)
        {
            if (config.isDebug())
            {
                Serial.println("==================================");
                Serial.println("[HTTPService] Falha no parse JSON.......");
                Serial.println(error.f_str());
            }
            delay(5000);

            return false;
        }

        if (doc["httpCode"].as<int>() == 200)
        {
            master = doc["result"]["uuid"].as<char *>();

            return true;
        }
        else
        {
            if (config.isDebug())
            {
                Serial.println("==================================");
                Serial.print("[HTTPService] Mensagem: ");
                Serial.println(doc["message"].as<char *>());
            }
            return false;
        }
    }

    return false;
}


/*
 * <descricao> Atualiza a tabela Monitoramento do banco de dados com as atualizacoes feitas nos equipamentos pelo ESP  <descricao/>
 * <parametros> luzes: indica o ultimo estado das luzes (ligado/desligado) <parametros/>
 * <parametros> condicionador: indica o ultimo estado do ar condicionado (ligado/desligado) <parametros/>
 * <retorno> string com nome do dispotivo recebido na requisicao ou os codigos IR <retorno/>
 */
/*bool enviarMonitoramento(bool luzes, bool condicionador) {

  bool atualizacaoMonitoramento = false;
  struct Monitoramento monitoramento = obterMonitoramentoByIdSala();
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status

    HTTPClient http;

    http.begin("http://italabs-002-site2.ctempurl.com/api/monitoramento"); //Specify the URL
    http.addHeader("Content-Type", "application/json");

    String id               = String(monitoramento.id);
    String luzesLiagadas    = String(luzes ? "true" : "false");
    String arCondicionado   = String(condicionador ? "true" : "false");
    String salaId           = String(monitoramento.salaId);

    String monitoramentoJson = "{ ";
          monitoramentoJson += "\"id\": "               + id             + ", ";
          monitoramentoJson += "\"luzes\": "            + luzesLiagadas  + ", ";
          monitoramentoJson += "\"arCondicionado\": "   + arCondicionado + ", ";
          monitoramentoJson += "\"salaId\": "           + salaId         + ", ";
          monitoramentoJson += " }";

    int httpResponseCode = http.PUT(monitoramentoJson);

    if (httpResponseCode == 200) 
      atualizacaoMonitoramento = true;
    else
      atualizacaoMonitoramento = false;

    http.end();
  }

  return atualizacaoMonitoramento;
}*/

/*
 * <descricao> Realiza requisicao ao servidor para obter as reservas da semana para a sala deste dispositivo <descricao/>   
 */
std::vector< struct Reserva> HTTPService::GetReservationsWeek() {
    
    Config config;
    HTTP http;
    String route;
    std::vector<struct Reserva> reservas;

    if (config.getRoute() == 1)
        route = "/horariosala/ReservasDeHoje/";
    else
        route = "/horariosala/ReservasDeHoje/";


    String routeService;
    String type = "GET";
    String params = "";
    String id_sala = "1";

    routeService.concat(route);
    routeService.concat(id_sala);

    String response = http.request(routeService, type, params);

    if (strstr(response.c_str(), "[ERROR]") == NULL)
    {
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, response);

        if (error)
        {
            if (config.isDebug())
            {
                Serial.println("==================================");
                Serial.println("[HTTPService] Falha no parse JSON.......");
                Serial.println(error.f_str());
            }
            delay(5000);
        }

        if (doc["httpCode"].as<int>() == 200)
        {
            JsonArray jsonSensors = doc["result"].as<JsonArray>();

            for (JsonVariant object : jsonSensors)
            {
                reservas.push_back(deserializeReserve(object));
            }
        }
        else
        {
            if (config.isDebug())
            {
                Serial.println("==================================");
                Serial.print("[HTTPService] Mensagem: ");
                Serial.println(doc["message"].as<char *>());
            }
        }
    }

    if (config.isDebug())
    {
        Serial.println("==================================");
        Serial.print("[HTTPService] count reservations: ");
        Serial.println(reservas.size());
    }
    
    return reservas;
}


/*
 * <descricao>   <descricao/>
 * <parametros>  <parametros/>
 */
struct Reserva HTTPService::deserializeReserve(JsonVariant reserve) {
   
   struct Reserva res;

   res.id = reserve["id"].as<int>();
   res.usuarioId = reserve["usuarioId"].as<int>();
   res.salaId = reserve["salaId"].as<int>();
   res.planejamento = reserve["planejamento"].as<int>();
   res.date = reserve["data"].as<String>();
   res.horarioInicio = reserve["horarioInicio"].as<String>();
   res.horarioFim = reserve["horarioFim"].as<String>();
   res.situacao = reserve["situacao"].as<String>();
   res.objetivo = reserve["objetivo"].as<String>();  

   //if (config.isDebug())
   //{
        Serial.println("==================================");
        Serial.print("[HTTPService] horarioInicio: ");
        Serial.println(res.horarioInicio);
        Serial.print("[HTTPService] horarioFim: ");
        Serial.println(res.horarioFim);
   //}

   return res;
}
