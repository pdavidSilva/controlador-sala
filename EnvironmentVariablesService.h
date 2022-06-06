#ifndef EnvironmentVariablesService_h
#define EnvironmentVariablesService_h

#include "Config.h"
#include <NTPClient.h>
#include <WiFiUDP.h>

using namespace std;

#define TYPE_LIGHT  0
#define TYPE_CONDITIONER  1 

class EnvironmentVariablesService 
{
  private: 
    String __currentTime;
    struct Monitoramento __monitoring;
    static vector<struct Reserva> __reservations; 
    static struct HardwareRecord __hardware; 
    static bool __receivedData;
    static String __message;
    String __startTimeLoadReservations;
    String __endTimeLoadReservations;
    static bool __uploadedToday;
    static bool __hasMovement;

  public: 
    EnvironmentVariablesService();

    String getCurrentTime();
    String setCurrentTime(String currentTime);

    struct Monitoramento getMonitoring();
    void setMonitoring(struct Monitoramento monitoramento);

    String getNtpFormatedTime();

    std::vector<struct Reserva> getReservations();
    void  setReservations(std::vector<struct Reserva> reservations);
    struct HardwareRecord getHardware();
    void setHardware(HardwareRecord hardware);

    String getMessage();
    void setMessage(String message);

    bool getReceivedData();
    void setReceivedData(bool receivedData);

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
    void CheckEnvironmentVariables();

    void awaitsReturn();
};

#endif
