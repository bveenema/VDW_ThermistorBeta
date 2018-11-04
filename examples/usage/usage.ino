// Example usage for Thermistor-Beta-VDW library by Ben Veenema.

#include "thermistor-beta-vdw.h"

// Initialize objects from the lib
ThermistorBetaVDW thermistorBetaVDW;

void setup() {
    // Call functions on initialized library objects that require hardware
    //(pin_t pin, int seriesResistor, int thermistorNominal, int temperatureNominal, int bCoef, uint32_t samples, uint32_t sampleDelay)
    thermistorBetaVDW.init(A3, 4700, 5000, 25, 3950, 100, 10);
}

void loop() {
    // Use the library's initialized objects and functions
    thermistorBetaVDW.update();
    if(thermistorBetaVDW.isValid()){
      static uint32_t lastTempWrite = 0;
      if(millis() - lastTempWrite > 1000){
        lastTempWrite = millis();
        Serial.printlnf("Temperature in K: %f",thermistorBetaVDW.readTempK());
        Serial.printlnf("Temperature in C: %f",thermistorBetaVDW.readTempC());
        Serial.printlnf("Temperature in F: %f",thermistorBetaVDW.readTempF());
      }
    }
}
