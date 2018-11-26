#include <Arduino.h>
#include <TridentTD_ESP32NVS.h>
#include "SaveSettings.h"

#define NVS_DEBUG

bool SaveSettings::Begin()
{
    // try to connect to the configured LAN
    // Load AP Settings from persistent storage
    #ifdef NVS_DEBUG
        Serial.println("Initializing NVS driver...");
    #endif
    // if NVS initializes correctly flags that is running ok
    Running = NVS.begin();
    #ifdef FIRST_TIME
    {
        SaveRouterSettings("x", "x"); // dummy to start the dictionary - need some rework
    }
    #endif
    #ifdef NVS_DEBUG
        if(Running)
            Serial.println("NVS: OK");
        else
            Serial.println("NVS: ERROR");
    #endif
    
    return Running;
}

void SaveSettings::ResetAllData()
{
    if(Running)
        NVS.eraseAll();      
}

void SaveSettings::SaveRouterSettings(String AP_Name, String password)
{
      #ifdef NVS_DEBUG
            //Serial.printf("NAME: %s - Pwd: %s", AP_Name, password);
        #endif
    if(Running){
        NVS.setString("SSID", AP_Name);
        NVS.setString("PASS", password);
        delay(50);
    } else {
        #ifdef NVS_DEBUG
            Serial.println("NVS service not running");
        #endif
    }
}

RouterSettings SaveSettings::ReadRouterSettings()
{
    RouterSettings output;
    output.SSID = NVS.getCharArray("SSID");
    output.PASSWORD = NVS.getCharArray("PASS");
    #ifdef NVS_DEBUG
        Serial.println("-----");
        Serial.println(output.SSID);
        Serial.println(output.PASSWORD);
    #endif
    return output;
}