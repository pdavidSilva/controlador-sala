#include "UtilsService.h"

EnvironmentVariablesService __environmentVariablesService;

UtilsService::UtilsService(){}

String UtilsService::mountPayload(String deviceType, String state, String command)
{
    String payload;
    payload.concat("{");
    payload.concat("\"type\":\"" + deviceType + "\", ");
    payload.concat("\"state\":\"" + state + "\", ");
    payload.concat("\"command\":\"" + command + "\"");
    payload.concat("}");

    return payload;
}

void UtilsService::updateMonitoring(String message)
{
    
    if(message == LZ_ON || message == LZ_OFF)
    {
        Monitoramento monitoringLight = __environmentVariablesService.getMonitoringLight();

        monitoringLight.estado = message == LZ_ON ? true : false;

        __environmentVariablesService.setMonitoringLight(monitoringLight);
    }

    if(message == AC_OFF || message == AC_ON)
    {
       Monitoramento monitoringConditioner = __environmentVariablesService.getMonitoringConditioner();    

       monitoringConditioner.estado = message == AC_ON ? true : false;

       __environmentVariablesService.setMonitoringConditioner(monitoringConditioner); 
    }

}