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

  // Configure BLE Service
  controller.configureBLEServer();
  controller.initBLETaskServer();	

  // Configure Socket Service
  // controller.initServerSocket();    
  // controller.startTaskWebSocket();
  controller.startTaskHttp();


  // Configure Environment Variables Service
  //controller.initEnvironmentVariables();
}

void loop() {
  //controller.environmentVariablesContinuousValidation(); 
}
