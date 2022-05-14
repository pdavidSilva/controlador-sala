#include "Config.h"

String __url;
String __ssid;
String __password;
int    __ledStatus;
int    __basetime;
int    __type;
int    __route;
bool   __debug;
vector<struct Reserva> Config::__reservations; 


Config::Config(){
    __url       = "http://italabs-002-site2.ctempurl.com/api";
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

std::vector<struct Reserva> Config::getReservations()
{
    return __reservations;
}

void Config::setReservations(std::vector<struct Reserva> reservations)
{
    __reservations = reservations;
}

