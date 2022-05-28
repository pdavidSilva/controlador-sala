#ifndef HTTPService_h
#define HTTPService_h

#include "Config.h"

class HTTPService
{
    public :
        HTTPService();
        void getInfoHardware(struct HardwareRecord&);
        bool registerHardware(struct HardwareRecord);
        void getSensors(struct HardwareRecord, String [], int &);
        void getDevices(struct HardwareRecord, String [], int &);
        std::vector<struct HardwareRecord> HTTPService::getSensors(HardwareRecord hardware);
        struct Actuator HTTPService::deserializeActuator(JsonVariant sensor);
        bool getMaster(struct HardwareRecord, String &);
        bool putMonitoring(struct Monitoramento monitoring);
        std::vector<Reserva> GetReservationsWeek();
        struct Reserva deserializeReserve(JsonVariant reserve);
        struct Monitoramento getMonitoringByUuid();
        String HTTPService::getComandosIrByIdSalaAndOperacao();
};

#endif
