#include "Config.h"

BLEServerService* bleConfig; 

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
  controller.setupBLEServer();

  // Configure Socket Service
  //controller.startTaskHttp();

  // Configure Environment Variables Service
  controller.setupEnvironmentVariables();
  controller.startEnvVariablesTask();
}

void loop() {
   
  if(controller.isTimeConnectSensors())
    controller.startBLETaskServer();
  else
    controller.startTaskHttp();

}
