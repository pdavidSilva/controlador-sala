#ifndef EquipmentService_h
#define EquipmentService_h

#define RELE 23

class EquipmentService 
{
  public: 
    bool __lightOn;
    
    bool getLightOn();
    void setLightOn(bool light);

    Vector<int> SplitIrComands(String data);
    String SplitGetIndex(String data, char separator, int index);
    void SendIrComand(Vector <int> codigo);
    void checkIrms();
    void checkOperationLights(String msg);
    void turnOnLights();
    void turnOffLights();
}

#endif