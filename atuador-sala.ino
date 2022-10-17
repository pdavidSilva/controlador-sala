/*#include "Global.h"
#include "Config.h"

HardwareRecord hardware;
Controller controller;
String master = "";

bool HAS_IR_TO_SEND = false;
String COMMAND_IR;
const uint16_t kIrLed = 12;
IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.

void setup() {
	Serial.begin(115200);
	irsend.begin();
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
  if(HAS_IR_TO_SEND) {
    Serial.println("[Loop] FOWARD TO SEND IR"); 
    HAS_IR_TO_SEND = false;
    controller.ExecuteCommandIR(COMMAND_IR);
  }

  delay(2000);
}
*/