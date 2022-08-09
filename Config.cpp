#include "Config.h"

String __url;
String __ssid;
String __password;
int    __ledStatus;
int    __basetime;
int    __type;
int    __route;
bool   __debug;

Config::Config(){
    __tokenApp  = "594ac3eb82b5080393ad5c426f61c1ed5ac53f90e1abebc15316888cf1c8f5fe";
    __url       = "https://webapi-cy5.conveyor.cloud/api";
    __debug     = true;
   //__ssid      = "3F1M 2";
   //__password  = "geladeira123";
    __ssid      = "Apartamento 201-2.4";
    __password  = "Ald50983021";
    __basetime  = 180000;
    __ledStatus = 2;
    __type  = TYPE_SENSOR;
    __route = 2;
    
    pinMode(__ledStatus, OUTPUT);
}

String Config::getTokenApp()
{ 
	return __tokenApp;
}

String Config::getUrl()
{ 
	return __url;
}

String Config::getSSID()
{
	return __ssid;
}

String Config::getPassword()
{
	return __password;
}

int	Config::getBaseTime()
{
	return __basetime;
}

int	Config::getType()
{
	return __type;
}

int	Config::getLedStatus() 
{
	return __ledStatus;
}

int Config::getRoute()
{
    return __route;
}
 
bool Config::isDebug() 
{
	return __debug;
}
