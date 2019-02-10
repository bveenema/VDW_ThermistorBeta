/*
  Thermistor Beta Library

  Copyright (c) 2018 Ben Veenema <ben@veenemadesignworks.com>
  All right reserved.
*/

#ifndef THERMISTOR_BETA_H
#define THERMISTOR_BETA_H

#include <math.h>

#include "application.h"

class VDW_ThermistorBeta {
public:
  VDW_ThermistorBeta();

  /*
  * Particle init, sets defaults: vcc=3.3, analogReference=3.3, adcMax=4095
  *
  * arg 1: pin: Photon analog pin
  * arg 2: seriesResistor: The ohms value of the fixed resistor (based on your hardware setup, usually 10k)
  * arg 3: thermistorNominal: Resistance at nominal temperature (will be documented with the thermistor, usually 10k)
  * arg 4: temperatureNominal: Temperature for nominal resistance in celcius (will be documented with the thermistor, assume 25 if not stated)
  * arg 5: bCoef: Beta coefficient (or constant) of the thermistor (will be documented with the thermistor, typically 3380, 3435, or 3950)
  * arg 6: samples: Number of analog samples to average (for smoothing)
  * arg 7: sampleDelay: Milliseconds between samples (for smoothing)
  */
  void init(pin_t pin, int seriesResistor, int thermistorNominal, int temperatureNominal, int bCoef, uint32_t samples, uint32_t sampleDelay);

  /*
  * General init, sets defaults: vcc=3.3, analogReference=3.3
  *
  * arg 1: pin: Photon analog pin
  * arg 2: seriesResistor: The ohms value of the fixed resistor (based on your hardware setup, usually 10k)
  * arg 3: adcMax: The maximum analog-to-digital convert value returned by analogRead (Photon is 4095 NOT the typical Arduino 1023!)
  * arg 4: thermistorNominal: Resistance at nominal temperature (will be documented with the thermistor, usually 10k)
  * arg 5: temperatureNominal: Temperature for nominal resistance in celcius (will be documented with the thermistor, assume 25 if not stated)
  * arg 6: bCoef: Beta coefficient (or constant) of the thermistor (will be documented with the thermistor, typically 3380, 3435, or 3950)
  * arg 7: samples: Number of analog samples to average (for smoothing)
  * arg 8: sampleDelay: Milliseconds between samples (for smoothing)
  */
  void init(pin_t pin, int seriesResistor, int adcMax, int thermistorNominal, int temperatureNominal, int bCoef, uint32_t samples, uint32_t sampleDelay);

  /*
  * Full init, no defaults (useful for ESP8266)
  *
  * arg 1: pin: Photon analog pin
  * arg 2: vcc: Input voltage
  * arg 3: analogReference: reference voltage. Typically the same as vcc, but not always (ie ESP8266=1.0)
  * arg 4: seriesResistor: The ohms value of the fixed resistor (based on your hardware setup, usually 10k)
  * arg 5: adcMax: The maximum analog-to-digital convert value returned by analogRead (Photon is 4095 NOT the typical Arduino 1023!)
  * arg 6: thermistorNominal: Resistance at nominal temperature (will be documented with the thermistor, usually 10k)
  * arg 7: temperatureNominal: Temperature for nominal resistance in celcius (will be documented with the thermistor, assume 25 if not stated)
  * arg 8: bCoef: Beta coefficient (or constant) of the thermistor (will be documented with the thermistor, typically 3380, 3435, or 3950)
  * arg 9: samples: Number of analog samples to average (for smoothing)
  * arg 10: sampleDelay: Milliseconds between samples (for smoothing)
  */
  void init(pin_t pin, double vcc, double analogReference, int seriesResistor, int adcMax, int thermistorNominal, int temperatureNominal, int bCoef, uint32_t samples, uint32_t sampleDelay);

  // Reads the ADC at the appropriate times
  void update();

  // Returns if readings are valid
  bool isValid();

  // Temperature in Kelvin
  double readTempK();

  // Temperature in Celsius
  double readTempC();

  // Temperature in Fahrenheit
  double readTempF();

private:
  // Calculates the average of the ADC samples
  double calcAverage();

  // convert ADC value to Kelvin
  double adcToK(double adc);

  // convert Kelvin to Celsius
  double kToC(double k);

  // convert Celsius to Fahrenheit
  double cToF(double c);

  pin_t _pin;
  double _vcc;
  double _analogReference;
  double _seriesResistor;
  double _adcMax;
  double _temperatureNominal;
  double _thermistorNominal;
  double _bCoef;
  uint32_t _numSamples;
  uint32_t _sampleDelay;

  // Samples Container
  struct{
    uint16_t readings[100] = {0};
    uint8_t index = 0;
    uint8_t length = 100;
    bool isValid = false;
    uint32_t lastRead = 0;
  } _samples;
};

#endif
