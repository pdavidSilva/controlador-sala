/*#include "Config.h"

HardwareRecord hardwareSensor;
MonitoringRecord monitoringRecord;
Controller controller;
String master = "";

DHT dht(4, DHT11);
float temperature; 
const int portaPresenca = GPIO_NUM_12;

void setup() {
	
	Serial.begin(115200);
	bool init = false;

	do {
		if ( controller.start(hardwareSensor) ) {
			if ( controller.registerHardware(hardwareSensor) ) {
				if(controller.getMaster(hardwareSensor, master))
				{
					Serial.print("master: ");
					Serial.println(master);
					// controller.startVisibleService(); 
					init = true;
				}
        controller.setHardwareConfig(hardwareSensor);

			}
		}
	} while( !init );
  
  controller.setupBLEClient("ESP_SENSOR", SENSOR);  
}

void loop() {
	Serial.println("loop");
    
	bool leitura = digitalRead(portaPresenca);
	temperature = dht.readTemperature();
	if(leitura) {
		monitoringRecord.hasPresent = "S";
	} else {
		monitoringRecord.hasPresent = "N";
	}
  
	if(!isnan(temperature) || temperature != NULL){
		monitoringRecord.temperature = temperature;
	} else {
		monitoringRecord.temperature = -1;
	}
  	delay(1000);
	controller.sendDataOfMonitoring(monitoringRecord);
	
	//sensors()
	//dispositivo()
	
    //Mestre -> Sensor de Presenca ()
    //Sensor de Presenca () -> Mestre

}*/