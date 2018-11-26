#include <arduinoFFT.h>
#include "SoundSensor.h"


bool SoundSensor::Begin()
{
    sampling_period = round(1000000*(1.0/samplingFrequency));
    FFT = arduinoFFT(vReal, vImag, SAMPLES, samplingFrequency);
    return true;
}

void SoundSensor::ReadSamples()
{
  for (uint16_t i = 0; i <SAMPLES; i++)
  {
    microseconds = micros(); 
    vReal[i] = analogRead(36);
    vImag[i] = 0;
    while(micros() < (microseconds + sampling_period)){ /* Do nothing ? */ }
  }
}

void SoundSensor::Analyse()
{
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
    // double peak = FFT.MajorPeak(vReal, SAMPLES, samplingFrequency);
    #ifdef SOUND_DEBUG
      PrintVectorSerial(SAMPLES>>1, SCL_PLOT);
    #endif
}

void SoundSensor::PrintVectorSerial(uint16_t bufferSize, uint8_t scaleType)
{
  for (uint16_t i = 0; i < bufferSize; i++)
  {
    /*
    double abscissa = 1.0F;
    switch (scaleType)
    {
      case SCL_INDEX:
        abscissa = (i * 1.0);
	      break;
      case SCL_TIME:
        abscissa = ((i * 1.0) / samplingFrequency);
        break;
      case SCL_FREQUENCY:
        abscissa = ((i * 1.0 * samplingFrequency) / SAMPLES);
	      break;
    }

    if(scaleType!=SCL_PLOT)
    {
      Serial.print(abscissa, 6);
      if(scaleType==SCL_FREQUENCY)
        Serial.print("Hz");
      Serial.print(" ");
    }
    */
    Serial.println(vReal[i], 1);
  }
  //Serial.println();
}

