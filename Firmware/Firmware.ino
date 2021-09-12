#include "dhtHandler.h"
#include "mpu6050Handler.h"
#include "currentSesnorHandler.h"
void setup(){
    Serial.begin(115200);
    delay(1000);
    setupDHT22();
    setupMPU6050();
    setupCurrentSensor();

}

void loop(){

}