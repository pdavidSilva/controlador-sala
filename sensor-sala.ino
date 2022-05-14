/*#include "Config.h"

HardwareRecord hardwareSensor;
MonitoringRecord monitoringRecord;
Controller controller;
String master = "";

DHT dht(4, DHT11);
int temperature; 
const int portaPresenca = GPIO_NUM_12;

//void initBLE(); 

void setup() {
	
	Serial.begin(115200);
	bool init = false;

	/*do {
		if ( controller.start(hardwareSensor) ) {
			if ( controller.registerHardware(hardwareSensor) ) {
				if(controller.getMaster(hardwareSensor, master))
				{
					Serial.print("master: ");
					Serial.println(master);
					// controller.startVisibleService(); 
					init = true;
				};
			}
		}
	} while( !init );*/

  //bleUuid = hardware.getUuid();
 /* controller.configureClient();  
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

}*/
