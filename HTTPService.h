#ifndef HTTPService_h
#define HTTPService_h

#include "Config.h"

class HTTPService
{
    public :
        HTTPService();
        void getInfoHardware(struct HardwareRecord&);
        bool registerHardware(struct HardwareRecord);
        //void getSensors(struct HardwareRecord, String [], int &);
        //std::vector<struct HardwareRecord> getSensors(struct HardwareRecord hardware);
        std::vector<struct HardwareRecord> getHardwares(struct HardwareRecord hardware);
        struct HardwareRecord deserializeDevice(JsonVariant sensor);
        bool getMaster(struct HardwareRecord, String &);
        bool putMonitoring(struct Monitoramento monitoring);
        std::vector<Reserva> GetReservationsWeek();
        struct Reserva deserializeReserve(JsonVariant reserve);
        struct Monitoramento getMonitoringByIdSalaAndEquipamento(String tipoEquipamento);
        String getComandosIrByIdSalaAndOperacao(String uuid);
        struct Solicitacao getSolicitacao(String tipoEquipamento);
        struct Solicitacao deserializeSolicitacao(int idSolicitacao,String payload);
        bool putSolicitacao(int idSolicitacao);
};

#endif
