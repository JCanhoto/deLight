#include <Arduino.h>
#include <TridentTD_ESP32NVS.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <SPIFFS.h>
#include <FS.h>
#include <ESPmDNS.h>

#include <functional>
#include <FS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "LedStrip.h"
#include "SaveSettings.h"
#include "ConfigConnection.h"
#include "SoundSensor.h"

#define LEDS_DEBUG
#define DEBUG


LedStrip Leds;
SoundSensor Sound;
ConfigConnection deLight_Connect;

bool one = false;

static int LED_blick_Timer = 500;
uint64_t count_PC = 0;

void setup() {
    Serial.begin(9600);
    delay(2000); // 3 second delay for recovery

    pinMode (LED_BUILTIN, OUTPUT);
    uint8_t pos[2] = {0,64};
    uint8_t bornRate[2] = {5,5};
    int8_t dirs[2] = {1,-1};
    CRGB color[2] = { {0,255,0},{255,0,0} };
   
    Leds.SetupOrigins(2, pos, bornRate, dirs, color, 1);
    Leds.SetupBaseColor(1000, CRGB(0,0,0), false);
    Leds.SetupGlobalFade(50, 2);
    Leds.Begin(129);
    Leds.SetBrightness(255);

    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);

    digitalWrite(LED_BUILTIN, LOW);
    one = false;

    deLight_Connect.Begin();
    Sound.Begin();
}

void loop() {

count_PC++;

    if(count_PC % LED_blick_Timer == 0 && one){
        // rooter = Settings.ReadRooterSettings();
        one = true;
        //serial.print("SSID: ");
        //Serial.println(rooter.SSID);
        //Serial.print("Password: ");
        //Serial.println(rooter.Password);
        digitalWrite(LED_BUILTIN, HIGH);
        //delay(LED_blick_Timer);
        Sound.ReadSamples();
    } else {
        if(count_PC % (LED_blick_Timer+1) == 0)  one = false;
        digitalWrite(LED_BUILTIN, LOW);
        //delay(LED_blick_Timer);
        Sound.Analyse();
    }

    /*
    if(Leds.Debug() == "Not OK") {
     digitalWrite (LED_BUILTIN, HIGH);	// turn on the LED
     delay(1000);
     digitalWrite (LED_BUILTIN, LOW);	// turn on the LED
     delay(300);
    }
    */
    // Leds.Run();
}