/*
  LedStrip.h - Library for controling colors in a RGB led strip.
  Created by Joaquim Canhoto, Set 2018
  Released into the public domain.
*/

#define FASTLED_ALLOW_INTERRUPTS 0

#ifndef LedStrip_h
#define LedStrip_h

#include <Arduino.h>
#include <FastLED.h>

#define MAX_LEDS 150
#define MAX_ORIGINS 10
#define COLOR_ORDER GRB
#define DEFAULT_BRIGHTNESS 255
#define FRAMES_PER_SECOND  500
#define DATA_PIN 22
//FIRE
#define COOLING  60
// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 90

class LedStrip {
public:
  void Begin(uint8_t numOfLeds);
  void SetupOrigins(uint8_t Quant, uint8_t* Positions, uint8_t* BornRates, int8_t *Directions, CRGB* Colors, uint8_t Cycle);
  void SetupBaseColor(uint16_t Cycle,  CRGB BaseColor, bool RandomCycle);
  void SetupGlobalFade(uint8_t FadeRate, uint8_t FadeCycle);
  void SetBrightness(uint8_t Bright);
  void Run(); 
  String Debug();

private:

  CRGB _leds[MAX_LEDS];  
  bool OneStep();
  void Rainbow();
  void Confetti();
  void AddGlitter(fract8 ChanceOfGlitter);
  void RainbowWithGlitter();
  void Bpm();
  void Juggle();
  void Fire2012(CPixelView<CRGB> Section, uint8_t Quant, int8_t InvertedDirection);
  void CircleOfFire();
  CRGB Fade(CRGB color, uint8_t rate); /* do the fading to the base color */
  
  
  uint8_t _numberOfLeds = MAX_LEDS;

  /* Controls the cycle without delays*/
  uint32_t _stepCounter = 0;
  uint8_t _dataPin;

  /*Number of origins*/
  uint8_t _numberOfOrigins=4;   
  uint8_t _origins[MAX_ORIGINS];
  uint8_t _bornRate[MAX_ORIGINS];
  CRGB _originsColors[MAX_ORIGINS];
  bool _randomOriginColor = false;
  uint8_t _animationCycle = 1;
  uint8_t _brightness = DEFAULT_BRIGHTNESS;
    
  /*Control origins Status*/
  int8_t _currDirection[MAX_ORIGINS];
  uint8_t _currPosition[MAX_ORIGINS];

  /*base Color*/
  CRGB _baseColor;
  bool _randomBaseColor = true;
  uint16_t _randomCycleBaseColor = 1000;
  uint8_t _fadeRate = 5; 
  uint8_t _fadeCycle = 1; 
  
  // rainbow
  uint8_t _gHue = 0;
  uint8_t _hueCycle = 2;
};

#endif