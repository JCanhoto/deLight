#include <Arduino.h>
#include "LedStrip.h"

void LedStrip::Begin(uint8_t numOfLeds)
{
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(_leds, numOfLeds).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(_brightness);
    _numberOfLeds = numOfLeds;
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
}



void LedStrip::SetupOrigins(uint8_t Quant, uint8_t* Positions, uint8_t* BornRates, int8_t *Directions, CRGB* Colors, uint8_t Cycle)
{
    if(Quant>MAX_ORIGINS) Quant = MAX_ORIGINS;
    _numberOfOrigins = Quant;
    for(uint8_t i = 0; i< Quant; i++)
    {
        _origins[i] = *Positions;
        _currPosition[i] = *Positions;
        _currDirection[i] = *Directions;
        _bornRate[i] = *BornRates;
        _originsColors[i] = *Colors;
        _animationCycle = Cycle;
        BornRates++;
        Positions++;
        Directions++;
        Colors++;
    }
}

String LedStrip::Debug()
{
    if(_origins[0]==10 && _origins[1]==20)
        return "OK";
    else
        return "Not OK";
}

void LedStrip::SetupBaseColor(uint16_t Cycle,  CRGB BaseColor, bool RandomCycle)
{
    _baseColor = BaseColor;
    _randomBaseColor = RandomCycle;
    _randomCycleBaseColor = Cycle;
}

void LedStrip::SetupGlobalFade(uint8_t FadeRate, uint8_t FadeCycle)
{
    _fadeRate = FadeRate; /*speed they convert to the base color*/
    _fadeCycle = FadeCycle; 
}

void LedStrip::Run()
{
    if (OneStep())
        FastLED.show();
    // Rainbow();
    // Confetti();
    // RainbowWithGlitter();
    // Bpm();
    // CircleOfFire();
    
     //Juggle();
    if(_stepCounter % _hueCycle == 0)
        _gHue++;

    FastLED.delay(1000/FRAMES_PER_SECOND); 
}

void LedStrip::Bpm()
{
    // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = PartyColors_p;
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for( int i = 0; i < _numberOfLeds; i++) { //9948
        _leds[i] = ColorFromPalette(palette, _gHue+(i*2), beat-_gHue+(i*10));
    }
}

void LedStrip::Juggle() 
{
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( _leds, _numberOfLeds, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    _leds[beatsin16(i+7,0,_numberOfLeds)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void LedStrip::Rainbow() 
{
    fill_rainbow( _leds, _numberOfLeds, _gHue);
}


void LedStrip::Confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( _leds, _numberOfLeds, 20);
  int pos = random16(_numberOfLeds);
  _leds[pos] += CHSV( _gHue + random8(64), 200, 255);
}

void LedStrip::RainbowWithGlitter() 
{
  Rainbow();
  AddGlitter(200);
}



void LedStrip::AddGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) { //50%
    _leds[ random16(_numberOfLeds) ] += CRGB::White;
  }
}

CRGB LedStrip::Fade(CRGB theColor, uint8_t rate)
{
    uint8_t R, G, B;
    R = lerp8by8(theColor.r, _baseColor.r, rate);
    G = lerp8by8(theColor.g, _baseColor.g, rate);
    B = lerp8by8(theColor.b, _baseColor.b, rate);

    return CRGB(R, G, B);
}

void LedStrip::SetBrightness(uint8_t Bright)
{
    FastLED.setBrightness(Bright);
}

bool LedStrip::OneStep()
{  
    bool updateSomething = false;
    // se tem fade --- aplicar
    if (_fadeRate > 0 && _stepCounter % _fadeCycle == 0) {
        updateSomething = true;
        for(int i = 0; i<_numberOfLeds; i++)
        {
            _leds[i] = Fade(_leds[i], _fadeRate);
        }
    }

    // teste de um algoritmo ultra simples
    if (_stepCounter % _animationCycle == 0)
    {
        for (int i = 0; i < _numberOfOrigins; i++)
        {
            if ((_currPosition[i] + _currDirection[i] >= _numberOfLeds-1) || (_currPosition[i] + _currDirection[i] <= 0))
                _currPosition[i] = _origins[i];
            else
                _currPosition[i] += _currDirection[i];

            if(_randomOriginColor)
                _leds[_currPosition[i]] = CRGB(random8(),random8(),random8());
            else
                _leds[_currPosition[i]] = _originsColors[i];

            updateSomething = true;
        }
    }

    if (_stepCounter % _randomCycleBaseColor == 0 && _randomBaseColor)
    {
        _baseColor = CRGB(random8(),random8(),random8());
    }

    if (_stepCounter >= 4294967924)
        _stepCounter = 0;
    else
        _stepCounter++;

  return updateSomething;
}

void LedStrip::CircleOfFire()
{
// são quatro secções
// no nosso exemplo, mas depois vemos isto de outra maneira
/*

sections[0] = CPixelView<CRGB>(_leds, 0, 33);
sections[1] = CPixelView<CRGB>(_leds, 34, 69);
sections[2] = CPixelView<CRGB>(_leds, 70, 103);
sections[3] = CPixelView<CRGB>(_leds, 104, _numberOfLeds-1);*/

Fire2012(CPixelView<CRGB>(_leds, 0, 33), 33, false);
Fire2012(CPixelView<CRGB>(_leds, 33, 69), 69-33, true);
Fire2012(CPixelView<CRGB>(_leds, 62, 96), 96-62, false);
Fire2012(CPixelView<CRGB>(_leds, 96, _numberOfLeds-1), _numberOfLeds-1 - 96, true);

}

void LedStrip::Fire2012(CPixelView<CRGB> Section, uint8_t Quant, int8_t InvertedDirection)
{
// Array of temperature readings at each simulation cell

  static byte heat[MAX_LEDS];
  // Step 1.  Cool down every cell a little
    for( int i = 0; i < Quant; i++) 
    {
      heat[i] = qsub8( heat[i], random8(0, ((COOLING * 10)/Quant) + 2));
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= Quant - 1; k >= 2; k--) 
    {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) 
    {
      int y = random8(7);
      heat[y] = qadd8(heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < Quant; j++) 
    {
      CRGB color = HeatColor(heat[j]);
      int pixelnumber;
      if( InvertedDirection ) {
        pixelnumber = (Quant-1) - j;
      } else {
        pixelnumber = j;
      }
      Section.leds[pixelnumber] = color;
      // _leds
    }
}