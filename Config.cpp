#include "Config.h"

String __url;
String __ssid;
String __password;
int    __ledStatus;
int    __basetime;
int    __type;
bool   __debug;

Config::Config(){
    __url       = "https://manager.homolog.space/api/admin/v1";
    __debug     = true;
    __ssid      = "VIVOFIBRA-0391";
    __password  = "p@91253856";
    __basetime  = 180000;
    __ledStatus = 2;
    __type  = 1;
    pinMode(__ledStatus, OUTPUT);
}

String Config::getUrl() const
{ 
	return __url;
}

String Config::getSSID() const
{
	return __ssid;
}

String Config::getPassword() const
{
	return __password;
}

int	Config::getBaseTime() const
{
	return __basetime;
}

int	Config::getType() const
{
	return __type;
}


int	Config::getLedStatus() const
{
	return __ledStatus;
}

bool Config::isDebug() const
{
	return __debug;
}

 
