#include "Config.h"
#include "MqttService.h"
#define MSG_BUFFER_SIZE (500)

Config __configAccess;
BLEServerService* __bleConfiguration; 
HTTPService __httpService;
EnvironmentVariablesService __environment;
UtilsService __utils;
PubSubClient * __client;

HardwareRecord __hardware;

char msg[MSG_BUFFER_SIZE];

bool MqttService::__messageReturned = false;
String MqttService::__message = "";

MqttService::MqttService(PubSubClient *client) {
  __client = client;
}


String MqttService::getMessage() {
    return  __message;
}

void MqttService::setMessage(String message) {
    __message = message;
}

bool MqttService::getMessageReturned() {
    return  __messageReturned;
}

void MqttService::startMqttService(PubSubClient *client, HardwareRecord hardware)
{
    __client = client;
    __hardware = hardware;
    xTaskCreate(this->monitorSolicitation, "monitorSolicitation", 8192, this, 5, NULL);
}

void MqttService::monitorSolicitation(void* _this)
{
    setup();
    MonitoringRequest request;
    while (true)
    {
        if (__configAccess.isDebug())
        {
            Serial.println("=======================================");
            Serial.println("[MqttService] Start");
        }

        if (!__client->connected()) {
            reconnect();
        }

        __client->loop();

        if (__configAccess.isDebug())
        {
            Serial.println("=======================================");
            Serial.println("[MqttService] End");
        }
    }
    
}

bool MqttService::connectToActuator(String uuidDevice) 
{
  Serial.println("[MqttService]: connectToActuator ACTUATOR : " + uuidDevice);
  bool deviceConnected = false;
  int i = 0;
  int count = 8;
            
  do
  { 
    i++;
    
    if (__configAccess.isDebug())
    {
      Serial.print("[MqttService]: attempt number: ");
      Serial.println(i);
    }
    
    deviceConnected = __bleConfiguration->connectToActuator(uuidDevice);
    
    if(deviceConnected)
      break;
    delay(2000);  
  } while(i < count);

  if( i >= count && !deviceConnected)
      Serial.println("[MqttService]: device not found");

  return deviceConnected;
}

void MqttService::executeSolicitation(MonitoringRequest request) 
{
    __bleConfiguration->setReceivedRequest(true);
    delay(1500);
    
    bool dispConnected = connectToActuator(request.uuid);

    if(dispConnected)
    {
        String payload = getMessageToSend(request);
        Serial.println("==================================");
        Serial.println("[MqttService] Sendig Payload: " + payload);

        std::vector<String> subStrings = __utils.splitPayload(payload, MAX_LENGTH_PACKET);

        String packet;
        for (packet : subStrings)
        {
            Serial.println("==================================");         
            Serial.println("[MqttService] Sendig packet: " + packet);
            __bleConfiguration->sendMessageToActuator(packet);
        }

        //awaitsReturn();

        __bleConfiguration->disconnectToActuator();
    }

    __bleConfiguration->setReceivedRequest(false);

    __utils.updateMonitoring(__message);

    if (__configAccess.isDebug())
    {
        Serial.println("==================================");
        Serial.println("[MqttService] Resposta BLE");
        Serial.println("[MqttService] recebeu retorno: " + __messageReturned);
        Serial.println("[MqttService] mensagem: " + __message);
    }

    __messageReturned = false;
    __message = "";  

}

String MqttService::getMessageToSend(MonitoringRequest request)
{
    String typeEquipament = "", state = "", command = "null";

    if(request.type == LUZES)
        typeEquipament = "LZ";
    else
    {
        typeEquipament = "AC";
        command = request.code;
    }

    if(request.acting == "True")
        state = "ON";
    else
        state = "OFF";

    return __utils.mountPayload(typeEquipament, state, command);
}

MonitoringRequest MqttService::deserealizeObject(String payload)
{
    MonitoringRequest request;

    DynamicJsonDocument doc(40965);
    deserializeJson(doc, payload);
    
    request.type = doc["type"].as<char *>();
    request.code = doc["code"].as<char *>();
    request.uuid = doc["uuid"].as<char *>();
    request.acting = doc["acting"].as<char *>();
    
    return request;
}

void MqttService::callback(char* topic, byte* payload, unsigned int length) {
    String data = "";

    if (__configAccess.isDebug())
    {
        Serial.println("=======================================");
        Serial.print("[MqttService] Message arrived [");
        Serial.print(topic);
        Serial.print("] ");
        Serial.print(" - length ");
        Serial.println(length);
    }
    if(length > 0) 
    {
        for (int i = 0; i < length; i++) 
        {
            data += (char)payload[i];
        }
        
        MonitoringRequest request = deserealizeObject(data);
        executeSolicitation(request);
        Serial.println("[MqttService] Message data: " + data);
    }
  }

void MqttService::reconnect() {

  while (!__client->connected()) {
    Serial.print("[MqttService] Attempting MQTT connection… DEVICE: " + __hardware.uuid);
    String __clientId = "ESP8266__client";
    String sub = "esp/" + __hardware.uuid;
    Serial.println("\n\nserver: " +  __configAccess.getMqttUser());
    Serial.println("pass: " +  __configAccess.getMqttPassword());
    
    if (__client->connect(__clientId.c_str(), "server", "Salas@2022")) {
      Serial.println("[MqttService] Connected");
      __client->subscribe(sub.c_str());
    } else {
      Serial.print("[MqttService] Failed, rc = ");
      Serial.print(__client->state());
      Serial.println("[MqttService] Try again in 5 seconds");
      delay(5000);
    }
  }
}

void MqttService::setup() {
    Serial.println("\n\nuser: " +  __configAccess.getMqttServer());
    Serial.print("\nport: " );
    Serial.println(__configAccess.getMqttPort());
    const char* mqtt_server = "5cca6ae0a1694e46b4f3459338a2e561.s2.eu.hivemq.cloud";

  __client->setServer(mqtt_server, 8883);
  __client->setCallback(callback);
}