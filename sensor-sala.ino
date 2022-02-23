#include "Config.h"
HardwareRecord hardware;
MonitoringRecord monitoringRecord;
Controller controller;
String master = "";

DHT dht(4, DHT11);
int temperature; 
const int portaPresenca = GPIO_NUM_12;

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
  initBLE();
}

void loop() {
	Serial.println("loop");

	bool leitura = digitalRead(portaPresenca);
	temperature = (int)dht.readTemperature();
	monitoringRecord.hasPresent = leitura;          
	monitoringRecord.temperature = temperature;

	controller.sendDataOfMonitoring(monitoringRecord);
	//sensors()
	//dispositivo()
	
  //Mestre -> Sensor de Presenca ()
  //Sensor de Presenca () -> Mestre

}
