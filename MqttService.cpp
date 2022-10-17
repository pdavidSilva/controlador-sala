#include "Config.h"
#include "MqttService.h"
#define MSG_BUFFER_SIZE (500)

const char *ROOT_CA PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";

Config __configAccess;
BLEServerService* __bleConfiguration; 
HTTPService __httpService;
EnvironmentVariablesService __environment;
UtilsService __utils;
WiFiClientSecure __espClient;
PubSubClient * __client;

HardwareRecord __hardware;

char msg[MSG_BUFFER_SIZE];

bool MqttService::__messageReturned = false;
String MqttService::__message = "";

MqttService::MqttService() {}


String MqttService::getMessage() {
    return  __message;
}

void MqttService::setMessage(String message) {
    __message = message;
}

bool MqttService::getMessageReturned() {
    return  __messageReturned;
}

void MqttService::startMqttService(HardwareRecord hardware)
{
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
    String clientId = "ESP8266Client";
    String sub = "esp/" + __hardware.uuid;
    if (__client->connect(clientId.c_str(), __configAccess.getMqttUser().c_str(), __configAccess.getPassword().c_str())) {
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
  __espClient.setCACert(ROOT_CA);
  __client = new PubSubClient(__espClient);

  __client->setServer(__configAccess.getMqttServer().c_str(), __configAccess.getMqttPort());
  __client->setCallback(callback);
}