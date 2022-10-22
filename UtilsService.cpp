#include "UtilsService.h"

EnvironmentVariablesService __environmentVariablesService;
HTTPService __httpUtilService;

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

void UtilsService::updateMonitoring(String message, String uuid)
{
    
    if(message == LZ_ON || message == LZ_OFF)
    {
        Monitoramento monitoringLight = __environmentVariablesService.getMonitoringLightByUuid(uuid);

        monitoringLight.estado = (message == LZ_ON);

        if(monitoringLight.equipamentoId > 0 && monitoringLight.id > 0) 
        {
           __environmentVariablesService.updateMonitoringLight(monitoringLight);
           __httpUtilService.putMonitoring(monitoringLight);
        }
    }

    if(message == AC_OFF || message == AC_ON)
    {
       Monitoramento monitoringConditioner = __environmentVariablesService.getMonitoringConditionerByUuid(uuid);    

       monitoringConditioner.estado = (message == AC_ON);

       if(monitoringConditioner.equipamentoId > 0 && monitoringConditioner.id > 0)
       {
           __environmentVariablesService.updateMonitoringConditioner(monitoringConditioner); 
           __httpUtilService.putMonitoring(monitoringConditioner);
       }
    }

}
 
std::vector<String> UtilsService::splitPayload(String payload, int maxSize)
{
    std::vector<String> subStrings;
    String packet = "";
    int index = 0;

    do
    {
        packet = packet + payload[index];

        if ((index != 0 && (index % MAX_LENGTH_PACKET) == 0) || (payload.length() - 1) == index)  
        {
           subStrings.push_back(packet);
           packet = "";
        }

        index++;

    } while(index < payload.length());

    subStrings.push_back(END_DATA);
 
    return subStrings;  
}