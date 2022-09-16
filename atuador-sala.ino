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

        		controller.setHardwareConfig(hardware);
			}
		}
	} while( !init ); 

  controller.configureClient("ESP_ATUADOR", ATUADOR);  
}

void loop() {
  Serial.println("[Loop] Await message");

  delay(5000);
}
