#ifndef BLESensorSettings_h
#define BLESensorSettings_h

#include "BLEDevice.h"

// Hardware
struct HardwareRecord
{
    int id;
    String token;
    String uuid; 
};

// Hardware
struct MonitoringRecord
{
    String hasPresent;
    float temperature; 
};

typedef struct BLEDeviceConnect
{
  BLEClient* pClient;
  BLERemoteCharacteristic* pRemoteCharacteristic;
  BLERemoteService* pRemoteService;
  String uuid = "";
  bool deviceFound = true;
};

typedef struct MonitoringRequest
{
  String type;
  String code;
  String uuid;
};


/*
 * Estrutura usada para guardar dados da reserva da sala
 */
typedef struct Reserva {
  int id;
  String  date;
  String  horarioInicio;
  String  horarioFim;
  String  situacao;
  String  objetivo;
  int usuarioId;
  int salaId;
  int planejamento;
};


#endif
