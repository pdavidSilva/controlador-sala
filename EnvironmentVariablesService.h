#ifndef EnvironmentVariablesService_h
#define EnvironmentVariablesService_h

#include "Config.h"

using namespace std;

#define TYPE_LIGHT  0
#define TYPE_CONDITIONER  1 
#define TURN_ON  1
#define TURN_OFF  2 
#define CHECK_TIME_TO_LOAD  600
#define CHECK_TIME_TO_TURN_OFF  900000 // 15 MINUTOS
#define CHECK_TIME_TO_AWAIT_RETURN  15000 
#define GET_TIME "GETTIME" 

class EnvironmentVariablesService 
{
  private: 
    String __currentTime;
    static vector<struct Reserva> __reservations; 
    static struct HardwareRecord __hardware; 
    String __startTimeLoadReservations;
    String __endTimeLoadReservations;
    static vector<struct Monitoramento> __monitoringConditioner;
    static vector<struct Monitoramento> __monitoringLight;
    static bool __uploadedToday;
    static bool __hasMovement;
    static unsigned long __lastTimeAttended;
    static unsigned long __lastTimeLoadReservations;

  public: 

    EnvironmentVariablesService();

    unsigned long getLastTimeAttended();
    void setLastTimeAttended(unsigned long time);

    String getCurrentTime();
    String setCurrentTime(String currentTime);

    std::vector<struct Monitoramento> getMonitoringLight();
    void setMonitoringLight(std::vector<struct Monitoramento>  monitoramento);

    std::vector<struct Monitoramento> getMonitoringConditioner();
    void setMonitoringConditioner(std::vector<struct Monitoramento>  monitoramento);

    String getNtpFormatedTime();
    
    std::vector<struct Reserva> getReservations();
    void  setReservations(std::vector<struct Reserva> reservations);
    struct HardwareRecord getHardware();
    void setHardware(HardwareRecord hardware);
    
    String getUuidActuator(int typeEquipment);

    bool getRoomDuringClassTime();
    
    void turnOnManagedDevices();
    void turnOffManagedDevices();

    void sendDataToActuator(String uuid, String message);
    void sendDataToActuator(int typeEquipment, String uuid, String message);

    void turnOffLight(String uuid);
    void turnOnLight(String uuid);

    void turnOnConditioner(String uuid);
    void turnOffConditioner(String uuid);

    void continuousValidation();
    void checkTimeToLoadReservations();
    void checkEnvironmentVariables();

    void awaitsReturn();

    unsigned long getLastTimeLoadReservations();

    void setLastTimeLoadReservations(unsigned long time);

    struct MonitoringRecord deserealizeData(String message);

    void initEnvironmentVariables(); 

    bool hasLightTurnOn(); 
    bool hasConditionerTurnOn();
    bool hasLightTurnOff(); 
    bool hasConditionerTurnOff();

    std::vector<String> getActuatorsByTypeEquipment(int typeEquipment);

    struct Monitoramento getMonitoringConditionerByUuid(String uuid);
    struct Monitoramento getMonitoringLightByUuid(String uuid);

    void updateMonitoringConditioner(struct Monitoramento monitoring);
    void updateMonitoringLight(struct Monitoramento monitoring);
};

#endif
