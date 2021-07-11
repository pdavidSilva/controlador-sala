#include "Config.h"

HardwareRecord hardware;
Controller controller;
String sensors[6];
String devices[6];
int indexSensors;
int indexDevices;

void setup() {
	
	Serial.begin(115200);
	bool init = false;

	indexSensors = 0;
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
	} while( !init );

	//controller.startMonitoring(); 
}

void loop() {

	//sensors()
	//dispositivo()
	
  //Mestre -> Sensor de Presenca ()
  //Sensor de Presenca () -> Mestre

}
