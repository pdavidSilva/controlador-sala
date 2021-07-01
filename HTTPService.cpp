#include "Config.h"
#include "HTTPService.h"

HTTPService::HTTPService(){}

void HTTPService::getInfoHardware(HardwareRecord &record){

    // hardware/MAC?api_token = TOKEN_BASE&tipo-hardware=TIPO
    
    Config config;
    Hardware hardware;
    HTTP http;
    
    String route = "/hardwaredesala/info/";
    String routeService;
    String type = "GET";
    String params = "";
    String token = "594ac3eb82b5080393ad5c426f61c1ed5ac53f90e1abebc15316888cf1c8f5fe";
    String response;

    routeService.concat(route);
    routeService.concat(hardware.getMacAddress());
    routeService.concat("?token=");
    routeService.concat(token);
    routeService.concat("&tipoHardware=");
    routeService.concat(config.getType());
    
    response = http.request(routeService, type, params);
    
    if (strstr(response.c_str(), "[ERROR]") == NULL){
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, response);

        if (error) {      
            if (config.isDebug())
            {
                Serial.println("==================================");
                Serial.println("[HTTPService] Falha no parse JSON.......");
                Serial.println(error.f_str());
            }
            delay(5000);
            
            return ; 
        }
        
        if(doc["httpCode"].as<int>() == 200){
          
            record.id = doc["result"]["id"].as<int>();
            record.token = doc["result"]["token"].as<char*>();
            record.uuid = doc["result"]["uuid"].as<char*>();

            return ;
        }else{
            if (config.isDebug())
            {
                Serial.println("==================================");
                Serial.print("[HTTPService] Mensagem: ");
                Serial.println(doc["message"].as<char*>());
            }
            return ; 
        }
        
    }else{
        return ; 
    }
      
    return ; 
    
}
