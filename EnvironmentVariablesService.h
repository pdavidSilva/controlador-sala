#ifndef EnvironmentVariablesService_h
#define EnvironmentVariablesService_h

#include "Config.h"

using namespace std;

#define TYPE_LIGHT  0
#define TYPE_CONDITIONER  1 
#define TIME_TO_LOAD  600000
#define TIME_TO_TURN_OFF  900000 // 15 MINUTOS
#define TIME_TO_AWAIT_RETURN  15000 
#define GET_TIME  "GETTIME"


class EnvironmentVariablesService 
{
  private: 
    static String __currentTime;
    static vector<struct Reserva> __reservations; 
    static struct HardwareRecord __hardware; 
    static bool __receivedData;
    static String __message;
    String __startTimeLoadReservations;
    String __endTimeLoadReservations;
    static struct Monitoramento __monitoringConditioner;
    static struct Monitoramento __monitoringLight;
    static bool __hasMovement;
    static bool __inClass;
    static unsigned long __lastTimeAttended;
    static unsigned long __lastTimeLoadReservations;

  public: 
    EnvironmentVariablesService();

    unsigned long getLastTimeAttended();
    void setLastTimeAttended(unsigned long time);

    String getCurrentTime();
    String setCurrentTime(String currentTime);

    struct Monitoramento getMonitoringLight();
    void setMonitoringLight(struct Monitoramento monitoramento);

    struct Monitoramento getMonitoringConditioner();
    void setMonitoringConditioner(struct Monitoramento monitoramento);
    
    std::vector<struct Reserva> getReservations();
    void  setReservations(std::vector<struct Reserva> reservations);
    struct HardwareRecord getHardware();
    void setHardware(HardwareRecord hardware);

    static bool getInClass();
    bool setInClass(bool inClass);

    String getMessage();
    void setMessage(String message);

    bool getReceivedData();
    void setReceivedData(bool receivedData);
    
    String getUuidActuator(int typeEquipment);

    bool getRoomDuringClassTime();
    
    void turnOnManagedDevices();
    void turnOffManagedDevices();

    void sendDataToActuator(String uuid, String message);
    void sendDataToActuator(int typeEquipment, String message);

    void turnOfLight();
    void turnOnLight();

    void turnOnConditioner();
    void turnOfConditioner();
    
    void startValidation();
    static void startValidationTaskImpl(void* _this);

    void continuousValidation();
    void checkTimeToLoadReservations();
    void checkEnvironmentVariables();

    void awaitsReturn();

    struct MonitoringRecord deserealizeData(String message);

    void initEnvironmentVariables(); 

    static unsigned long getLastTimeLoadReservations();

    static void setLastTimeLoadReservations(unsigned long time);

};

#endif
