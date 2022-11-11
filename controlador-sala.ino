#include "Config.h"

BLEServerService* bleConfig; 

HardwareRecord hardware;
Controller controller;
WiFiService wiFiService;
WiFiClientSecure espClient;
PubSubClient * client;

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

  espClient.setCACert(ROOT_CA);
  client = new PubSubClient(espClient);
  // Configure Mqtt Service

  //Configure BLE Service
  controller.setupBLEServer();
  controller.startBLETaskServer();	

  controller.startTaskMqtt(client, hardware);

  //Configure Environment Variables Service
  controller.initEnvironmentVariables();
}

void loop() {
   controller.environmentVariablesContinuousValidation(); 
 }