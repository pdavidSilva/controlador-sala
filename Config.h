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

    public : 
        
        Config();
        
        String getUrl() const;
        String getSSID() const;
        String getPassword() const;
        int    getType() const;
        int    getBaseTime() const;
        int    getLedStatus() const;
        bool   isDebug() const;
};

#endif
