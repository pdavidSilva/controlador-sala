#ifndef EquipmentService_h
#define EquipmentService_h

#include "Config.h"
#define RELE 19
#define pinSCT 14 
#define tensao 127

const uint16_t kIrLed = 12;

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
    String executeActionFromController(String data);
};

#endif
