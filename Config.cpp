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
    __url       = "https://manager.homolog.space/api/admin/v1";
    __debug     = true;
    __ssid      = "VIVOFIBRA-5F70";
    __password  = "F03C999054";
    __basetime  = 180000;
    __ledStatus = 2;
    __type  = 1;
    __route = 1;
    
    pinMode(__ledStatus, OUTPUT);
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
