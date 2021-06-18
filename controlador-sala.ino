#include "Config.h"

Controller controller;

void setup() {
  
  Serial.begin(115200);
  delay(4000);

  controller.start();
}

void loop() {
   
  
}
