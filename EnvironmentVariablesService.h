#ifndef EnvironmentVariablesService_h
#define EnvironmentVariablesService_h

#include <NTPClient.h>

#define TYPE_LIGHT  0
#define TYPE_CONDITIONER  1 

class EnvironmentVariablesService 
{
  private: 
    String __currentTime;
    struct Monitoramento monitoring;
    static vector<struct Reserva> EnvironmentVariablesService::__reservations; 
    static HardwareRecord EnvironmentVariablesService::__hardware; 
    String EnvironmentVariablesService::__startTimeLoadReservations;
    String EnvironmentVariablesService::__endTimeLoadReservations;
    bool EnvironmentVariablesService::__uploadedToday;
    NTPClient EnvironmentVariablesService::__ntp;
    WiFiUDP EnvironmentVariablesService::__udp;

  public: 
   void EnvironmentVariablesService()

    String getCurrentTime();
    String setCurrentTime(String currentTime);

    struct Monitoramento getMonitoring();
    void setMonitoring(struct Monitoramento);

    String getNtpFormatedTime();

    std::vector<struct Reserva> getReservations();
    void  setReservations(std::vector<struct Reserva> reservations);
    HardwareRecord getHardware();
    void setHardware(HardwareRecord hardware);

    void turnOnManagedDevices();
    void turnOffManagedDevices();

    bool sendDataToActuator(String uuid, String message);
    void sendDataToActuator(int typeEquipment, String message);

    void turnOfLight();
    void turnOnLight();

    void turnOnConditioner();
    void turnOfConditioner();

    void continuousValidation();
    void checkTimeToLoadReservations();

    void awaitsReturn();
};

#endif