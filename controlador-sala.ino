#include "Config.h"
#include "BLEServerService.h"
#include "BLESensorService.h"

HardwareRecord hardware;
Controller controller;
String sensors[6];
String devices[6];
int indexSensors;
int indexDevices;

BLEServerService* bleConfig; 

void setup() {
	
	Serial.begin(115200);
	bool init = false;

  /*bleConfig = new BLEServerService();
  bleConfig->addSensor("4fafc201-1fb5-459e-8fcc-c5c9c3319222");
  bleConfig->addSensor("4fafc201-1fb5-459e-8fcc-c5c9c3319111");
  bleConfig->addSensor("4fafc201-1fb5-459e-8fcc-c5c9c3319333");

  bleConfig->addActuator("4fafc201-1fb5-459e-8fcc-c5c9c3319444");
  bleConfig->addActuator("4fafc201-1fb5-459e-8fcc-c5c9c3319555");
  bleConfig->addActuator("4fafc201-1fb5-459e-8fcc-c5c9c3319666");
  
  bleConfig->initBLE();  
  bleConfig->scanDevices();
  bleConfig->populateMap();

  delay(2000);
  bleConfig->startTask();*/

  initBLE();

	/*indexSensors = 0;
	indexDevices = 0;

	do {
		if ( controller.start(hardware) ) {
			if ( controller.registerHardware(hardware) ) {
				controller.getSensors(hardware, sensors, indexSensors);
        Serial.print("Sensors: ");
        Serial.println(indexSensors);
				//controller.getDevices(hardware, devices, indexDevices);
    
				//if( indexSensors > 0 && indexDevices > 0 ) {				
					//controller.startServer();
					// if (controller.scan()) {
					// 	init = true;
					// }
				//}
			}
		}
	} while( !init );*/

	//controller.startMonitoring(); 
}

void loop() {

  sendDataOfMonitoring();
	//sensors()
	//dispositivo()
	
  //Mestre -> Sensor de Presenca ()
  //Sensor de Presenca () -> Mestre

  /*bleConfig->setReceivedRequest(false);
  delay(4000);
  
  bleConfig->setReceivedRequest(true);
  delay(4000);*/
}
