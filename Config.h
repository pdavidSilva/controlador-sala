
#ifndef Config_h
#define Config_h

#include <Arduino.h>
#include <String>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "Controller.h"
#include "WiFiService.h"
#include "HTTP.h"
#include "Structs.h"
#include "HTTPService.h"
#include "Hardware.h"
#include "DHT.h"
#include "ClientSocketService.h"

class Config 
{
    private :
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
