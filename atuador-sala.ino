/*#include "Global.h"
#include "Config.h"

HardwareRecord hardware;
WiFiService wiFiService;
Controller controller;
String master = "";

bool SEND_DATA = false;
String COMMAND;
const uint16_t kIrLed = 12;
IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.

void setup() {
	Serial.begin(115200);
	irsend.begin();
	bool init = false;

  wiFiService.connect();

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

  controller.setupBLEClient("ESP_ATUADOR", ATUADOR);  
}

void loop() {
  Serial.println("[Loop] Await message"); 
  if(SEND_DATA) {
    Serial.println("[Loop] FOWARD TO SEND IR"); 
    SEND_DATA = false;
    controller.ExecuteCommandIR(COMMAND);
  }

  delay(500);
}
*/