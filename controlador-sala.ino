/*#include "Config.h"
//#include "BLEServerService.h"

BLEServerService* bleConfig; 
HardwareRecord hardware;
Controller controller;
WiFiService wiFiService;

String sensors[6];
String devices[6];
int indexSensors;
int indexDevices;

void setup() {
	
	Serial.begin(115200);
	bool init = false;

  sensors[0] = "63e21b8d-9fc0-4246-9b4c-c16bc94889e6";
	indexSensors = 1;
  devices[0] = "36938872-c3ca-11ec-9d64-0242ac120002";
	indexDevices = 1;

	wiFiService.connect();

	/* do {
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
	} while( !init ); */

  /*for(int i = 0; i < indexSensors; i++)
    bleConfig->addSensor(sensors[i]);
    
  for(int i = 0; i < indexDevices; i++)
    bleConfig->addActuator(devices[i]);

  controller.configureServer(); 
  controller.initBleTaskServer();
  
  /*bleConfig->initBLE();  
  bleConfig->scanDevices();
  bleConfig->populateMap();
  
  delay(2000);
  bleConfig->startTask();*/

  /*controller.configureServer();
  controller.initBleTaskServer();	

  //controller.startMonitoring(); 

  controller.initServerSocket();    
  controller.startTaskWebSocket(); 
}

void loop() {

	//sensors()
	//dispositivo()
	
  //Mestre -> Sensor de Presenca ()
  //Sensor de Presenca () -> Mestre
}*/
