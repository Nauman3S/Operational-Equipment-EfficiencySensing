#include "EmonLib.h"
#include <driver/adc.h>
#define ADC_INPUT 34
#define HOME_VOLTAGE 247.0
#define ADC_BITS 10
#define ADC_COUNTS (1 << ADC_BITS)

EnergyMonitor emon1;

unsigned long timeFinishedSetup = 0;

void setupCurrentSensor()
{
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
    analogReadResolution(10);
    emon1.current(ADC_INPUT, 30);
    timeFinishedSetup = millis();
}

String getCurrentWatts()
{

    double amps = emon1.calcIrms(1480); // Calculate Irms only
    double watt = amps * HOME_VOLTAGE;
    String v = String(amps); //+ String(",") + String(watt);

    return v;
}