/*
  photon-thermistor.cpp - Photon Thermistor Library

  Copyright (c) 2018 Paul Cowan <paul@monospacesoftware.com>
  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "thermistor-beta-vdw.h"

#define ABS_ZERO -273.15

ThermistorBetaVDW::ThermistorBetaVDW(){}

void ThermistorBetaVDW::init(pin_t pin, int seriesResistor, int thermistorNominal, int temperatureNominal, int bCoef, uint32_t samples, uint32_t sampleDelay){
  init(pin, 3.3, 3.3, seriesResistor, 4095, thermistorNominal, temperatureNominal, bCoef, samples, sampleDelay);
}

void ThermistorBetaVDW::init(pin_t pin, int seriesResistor, int adcMax, int thermistorNominal, int temperatureNominal, int bCoef, uint32_t samples, uint32_t sampleDelay){
  init(pin, 3.3, 3.3, seriesResistor, adcMax, thermistorNominal, temperatureNominal, bCoef, samples, sampleDelay);
}

void ThermistorBetaVDW::init(pin_t pin, double vcc, double analogReference, int seriesResistor, int adcMax, int thermistorNominal, int temperatureNominal, int bCoef, uint32_t samples, uint32_t sampleDelay){
  _pin = pin;
  _vcc = vcc;
  _analogReference = analogReference;
  _seriesResistor = seriesResistor;
  _adcMax = adcMax;
  _thermistorNominal = thermistorNominal;
  _temperatureNominal = temperatureNominal;
  _bCoef = bCoef;
  (samples <= 100) ? _numSamples = samples : _numSamples = 100;
  _sampleDelay = sampleDelay;

  pinMode(_pin, INPUT);
}

void ThermistorBetaVDW::update(){
  // check if ADC read is due
  if(millis() - _samples.lastRead > _sampleDelay){
    _samples.lastRead = millis();
    _samples.readings[_samples.index] = analogRead(_pin);
    _samples.index++;
    if(_samples.index >= _numSamples){
      _samples.isValid = true;
      _samples.index = 0;
    }
  }
}

bool ThermistorBetaVDW::isValid(){
  return _samples.isValid;
}

double ThermistorBetaVDW::calcAverage() {
  unsigned int sum = 0;
  for(unsigned int i=0; i<_numSamples; i++) {
    sum += _samples.readings[i];
  }

  return (1. * sum) / _numSamples;
}

double ThermistorBetaVDW::readTempK(){
	return adcToK(calcAverage());
}

double ThermistorBetaVDW::readTempC(){
  return kToC(readTempK());
}

double ThermistorBetaVDW::readTempF(){
  return cToF(readTempC());
}

double ThermistorBetaVDW::adcToK(double adc){
  double Vadc = 3.3 * (adc/4095.0);
	// double resistance = -1.0 * (_analogReference * _seriesResistor * adc) / (_analogReference * adc - _vcc * _adcMax);
  double resistance = ((_vcc * _seriesResistor) / Vadc) - _seriesResistor; 
  double steinhart = (1.0 / (_temperatureNominal - ABS_ZERO)) + (1.0 / _bCoef) * log(resistance / _thermistorNominal);
  double kelvin = 1.0 / steinhart;
  return kelvin;
}

double ThermistorBetaVDW::kToC(double k){
	double c = k + ABS_ZERO;
	return c;
}

double ThermistorBetaVDW::cToF(double c){
	return (c * 1.8) + 32;
}
