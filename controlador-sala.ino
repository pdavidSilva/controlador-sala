#include "Config.h"

BLEServerService* bleConfig; 
HardwareRecord hardware;
Controller controller;
EnvironmentVariablesService environment;
WiFiService wiFiService;

void setup() {
	
	Serial.begin(115200);
	bool init = false;

  wiFiService.connect();

	 /*do {
		if ( controller.start(hardware) ) {
			if ( controller.registerHardware(hardware) ) {

        controller.setHardwareConfig(hardware);
        controller.fillHardwares(hardware);

				//if( indexSensors > 0 && indexDevices > 0 ) {				
					//controller.startServer();
					// if (controller.scan()) {
					// 	init = true;
					// }
				//}
			}
		}
	} while( !init );*/

  // mocks
  hardware.uuid = "36938872-c3ca-11ec-9d64-0242ac120002";
  hardware.salaId = 1;
  controller.setHardwareConfig(hardware);

  /*sensors[0] = "73d67fd6-1433-4bf3-b52a-7532a3447bd3";
  indexSensors = 1;
  
  for(int i = 0; i < indexSensors; i++)
    bleConfig->addSensor(sensors[i]);

  HardwareRecord atuador;
  atuador.uuid = "63e21b8d-9fc0-4246-9b4c-c16bc94889e6";
  atuador.salaId = 1;
  atuador.typeHardwareId = 1;
  atuador.typeEquipment = 1;  
  
  bleConfig->addActuator(atuador);

  controller.configureServer();
  controller.initBleTaskServer();	*/

  //controller.startMonitoring(); 

  /*controller.initServerSocket();    
  controller.startTaskWebSocket();*/

  controller.fillHardwares(hardware);
}

void loop() {
  //controller.initEnvironmentVariables();
  //controller.environmentVariablesContinuousValidation(); 
}
