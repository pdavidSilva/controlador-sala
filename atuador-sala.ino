#include "Config.h"

HardwareRecord hardware;
Controller controller;
String master = "";

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
					init = true;
				}

        		controller.fillHardwares(hardware);
			}
		}
	} while( !init ); 

  /*
   * HardwareRecord hardware;
   * hardware.uuid = "63e21b8d-9fc0-4246-9b4c-c16bc94889e6";
   * hardware.salaId = 1;
   * controller.setHardwareConfig(hardware);
   */  
  
  controller.configureClient("ESP_ATUADOR", ATUADOR);  
}

void loop() {
	Serial.println("loop");

  delay(5000);
}            
