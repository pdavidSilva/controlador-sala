#include "stdint.h"

#ifndef Config_h
#define Config_h

#include "Enums.h"
#include <Arduino.h>
#include <String>
#include <WiFi.h>
#include <HTTPClient.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include "EmonLib.h"
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUDP.h>
#include <PubSubClient.h>
#include "Controller.h"
#include "WiFiService.h"
#include "HTTP.h"
#include "Structs.h"
#include "Defines.h"
#include "HTTPService.h"
#include "Hardware.h"
#include "DHT.h"
#include "BLEServerService.h"
#include "MqttService.h"
#include "EnvironmentVariablesService.h"
#include "UtilsService.h"

class Config 
{
    private :
        String __tokenApp;
        String __url;
        String __ssid;
        String __password;
        char* __mqttServer;
        uint16_t __mqttPort;
        String __mqttUser;
        String __mqttPassword;
        int    __ledStatus;
        int    __basetime;
        bool   __debug;
        int    __type;
        int    __route;
        int    __wifiFailAttempts;
        int    __commandSendAttempts;
        int    __timesToHasOne;
    public : 
        
        Config();
        
        String getTokenApp();
        String getUrl();
        String getSSID();
        String getWIFIPassword();
        char* getMqttServer();
        uint16_t getMqttPort();
        String getMqttUser();
        String getMqttPassword();
        int    getType();
        int    getBaseTime();
        int    getLedStatus();
        bool   isDebug();
        int    getRoute();        
        int    getWifiFailAttempts();
        int    getCommandSendAttempts();
        int    getTimesToHasOne();
};

#endif
