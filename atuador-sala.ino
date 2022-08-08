/*
#include "Config.h"

//CommandDevice commandDevice;
HardwareRecord hardware;
Controller controller;
String master = "";

//void initBLE(); 

void setup() {
	
	Serial.begin(115200);
	bool init = false;

	do {
		if ( controller.start(hardware) ) {
			if ( controller.registerHardware(hardware) ) {
				if(controller.getMaster(hardware, master))
				{
					Serial.print("master: ");
					Serial.println(master);
					// controller.startVisibleService(); 
					init = true;
				};
			}
		}
	} while( !init );

  //bleUuid = hardware.getUuid();
  controller.configureClient("ESP_ATUADOR", ATUADOR);  
}

void loop() {
	Serial.println("loop");

	//sensors()
	//dispositivo()
	
  //Mestre -> Sensor de Presenca ()
  //Sensor de Presenca () -> Mestre

}        */            
