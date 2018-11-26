#ifndef SoundSensor_h
#define SoundSensor_h

#include <Arduino.h>
#include <arduinoFFT.h>

#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03
#define SAMPLES 128 //This value MUST ALWAYS be a power of 2

#define MIC_PIN 36

#define SOUND_DEBUG

/*
EXEMPLO
https://github.com/G6EJD/ESP32-8266-Audio-Spectrum-Display/blob/master/ESP32_Spectrum_Display_02.ino
*/

class SoundSensor {
    public:
        bool Begin();
        void ReadSamples();
        void Analyse();
        void PrintVectorSerial(uint16_t bufferSize, uint8_t scaleType);
        
    private:
        // configurarion and settings
        arduinoFFT FFT;
        
        const double signalFrequency = 1000;
        const double samplingFrequency = 5000;
        const uint8_t amplitude = 100;

        uint64_t microseconds;
        uint16_t sampling_period;
        double vReal[SAMPLES];
        double vImag[SAMPLES];
};

#endif



