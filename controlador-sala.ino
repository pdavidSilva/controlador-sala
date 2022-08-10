#include "Config.h"

BLEServerService* bleConfig; 
//EnvironmentVariablesService environment;

HardwareRecord hardware;
Controller controller;
WiFiService wiFiService;

void setup() {
	
	Serial.begin(115200);
	bool init = false;

  wiFiService.connect();

	do {
		if ( controller.start(hardware) ) {
			if ( controller.registerHardware(hardware) ) {

        controller.setHardwareConfig(hardware);
        controller.fillHardwares(hardware);

				if ( controller.loadedDevices() )				
					init = true;

			}
		}
	} while( !init ); 
  
  // mocks
  /* *
   * hardware.uuid = "36938872-c3ca-11ec-9d64-0242ac120002";
   * hardware.salaId = 1;
   * controller.setHardwareConfig(hardware);
   * 
   * bleConfig->addSensor("73d67fd6-1433-4bf3-b52a-7532a3447bd3");
   * 
   *  HardwareRecord atuador;
   *  atuador.uuid = "63e21b8d-9fc0-4246-9b4c-c16bc94889e6";
   *  atuador.salaId = 1;
   *  atuador.typeHardwareId = 1;
   *  atuador.typeEquipment = 1; 
   * 
   *  bleConfig->addActuator(atuador);
   * */ 
   
  // Configure BLE Service
  controller.configureBLEServer();
  controller.initBLETaskServer();	

  // Configure Socket Service
  controller.initServerSocket();    
  controller.startTaskWebSocket();

  // Configure Environment Variables Service
  controller.initEnvironmentVariables();
}

void loop() {
  controller.environmentVariablesContinuousValidation(); 
}
