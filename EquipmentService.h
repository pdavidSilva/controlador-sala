#ifndef EquipmentService_h
#define EquipmentService_h

#include <Vector.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include "EmonLib.h"

#define RELE 23
#define kIrLed 12
#define pinSCT 14 //Pino analógico conectado ao SCT-013
#define tensao 127

IRsend irsend(kIrLed);

class EquipmentService 
{
  public: 
    
    bool __lightOn;
    EnergyMonitor __sct13;
    
    EquipmentService();

    EnergyMonitor geSct();
    void setSct(EnergyMonitor light);

    bool getLightOn();
    void setLightOn(bool light);

    Vector<int> SplitIrComands(String data);
    String SplitGetIndex(String data, char separator, int index);
    void SendIrComand(Vector <int> codigo);
    bool checkIrms();
    void checkOperationLights(String msg);
    void turnOnLights();
    void turnOffLights();
}

#endif