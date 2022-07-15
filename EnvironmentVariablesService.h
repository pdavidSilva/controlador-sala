#ifndef EnvironmentVariablesService_h
#define EnvironmentVariablesService_h

#include "Config.h"

using namespace std;

#define TYPE_LIGHT  0
#define TYPE_CONDITIONER  1 
#define CHECK_TIME_TO_LOAD  60

class EnvironmentVariablesService 
{
  private: 
    String __currentTime;
    static vector<struct Reserva> __reservations; 
    static struct HardwareRecord __hardware; 
    static bool __receivedData;
    static String __message;
    String __startTimeLoadReservations;
    String __endTimeLoadReservations;
    static struct Monitoramento __monitoringConditioner;
    static struct Monitoramento __monitoringLight;
    static bool __uploadedToday;
    static bool __hasMovement;

  public: 
    EnvironmentVariablesService();

    String getCurrentTime();
    String setCurrentTime(String currentTime);

    struct Monitoramento getMonitoringLight();
    void setMonitoringLight(struct Monitoramento monitoramento);

    struct Monitoramento getMonitoringConditioner();
    void setMonitoringConditioner(struct Monitoramento monitoramento);

    String getNtpFormatedTime();

    std::vector<struct Reserva> getReservations();
    void  setReservations(std::vector<struct Reserva> reservations);
    struct HardwareRecord getHardware();
    void setHardware(HardwareRecord hardware);

    String getMessage();
    void setMessage(String message);

    bool getReceivedData();
    void setReceivedData(bool receivedData);
    
    String getUuidActuator(int typeEquipment);

    void turnOnManagedDevices();
    void turnOffManagedDevices();

    void sendDataToActuator(String uuid, String message);
    void sendDataToActuator(int typeEquipment, String message);

    void turnOfLight();
    void turnOnLight();

    void turnOnConditioner();
    void turnOfConditioner();

    void continuousValidation();
    void checkTimeToLoadReservations();
    void checkEnvironmentVariables();

    void awaitsReturn();

    void initEnvironmentVariables(); 
};

#endif
