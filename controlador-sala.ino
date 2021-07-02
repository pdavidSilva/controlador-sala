#include "Config.h"

HardwareRecord hardware;
Controller controller;
String sensors[6];
String devices[6];
int indexSensor;
int indexDevices;

void setup() {
	Serial.begin(115200);
	bool init = false;

	indexSensor = 0;
	indexDevices = 0;

	do {
		
		if (controller.start(hardware)){
			if (controller.registerHardware(hardware)){
				controller.getSensors(hardware, sensors, indexSensor);
				controller.getDevices(hardware, devices, indexDevices);
			}
		}

	}while(!init);

	controller.startBle();

}

void loop() {
   

	sensors()
	dispositivo()

  //Mestre -> Sensor de Presenca ()
  //Sensor de Presenca () -> Mestre

}
