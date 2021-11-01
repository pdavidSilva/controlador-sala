#include "Config.h"
#include "BLESensorService.h"

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
					// controller.startVisibleService(); 
					init = true;
				};
			}
		}
	} while( !init );

  bleUuid = hardware.getUuid();
  initBLE();
}

void loop() {
	Serial.println("loop");

   sendDataOfMonitoring();
	//sensors()
	//dispositivo()
	
  //Mestre -> Sensor de Presenca ()
  //Sensor de Presenca () -> Mestre

}
