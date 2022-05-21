#ifndef EnvironmentVariablesService_h
#define EnvironmentVariablesService_h

class EnvironmentVariablesService 
{
  private: 
    String __currentTime;
    struct Monitoramento monitoring;
    static vector<struct Reserva> EnvironmentVariablesService::__reservations; 
    static HardwareRecord EnvironmentVariablesService::__hardware; 

  public: 
    String getCurrentTime();
    String setCurrentTime(String currentTime);

    struct Monitoramento getMonitoring();
    void setMonitoring(struct Monitoramento);

    String getNtpFormatedTime();

    std::vector<struct Reserva> getReservations();
    void  setReservations(std::vector<struct Reserva> reservations);
    HardwareRecord getHardware();
    void setHardware(HardwareRecord hardware);

    bool senDataToActuator(struct Monitoramento monitoring);

    void awaitsReturn();
};

#endif