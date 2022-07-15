
#ifndef Config_h
#define Config_h

#include "Enums.h"
#include <Arduino.h>
#include <String>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Vector.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include "EmonLib.h"
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUDP.h>
#include "Controller.h"
#include "WiFiService.h"
#include "HTTP.h"
#include "Structs.h"
#include "HTTPService.h"
#include "Hardware.h"
#include "DHT.h"
#include "BLEServerService.h"
#include "ClientSocketService.h"
#include "EnvironmentVariablesService.h"
#include "UtilsService.h"

class Config 
{
    private :
        String __tokenApp;
        String __url;
        String __ssid;
        String __password;
        int    __ledStatus;
        int    __basetime;
        bool   __debug;
        int    __type;
        int    __route;
        
    public : 
        
        Config();
        
        String getTokenApp();
        String getUrl();
        String getSSID();
        String getPassword();
        int    getType();
        int    getBaseTime();
        int    getLedStatus();
        bool   isDebug();
        int    getRoute();
};

#endif
