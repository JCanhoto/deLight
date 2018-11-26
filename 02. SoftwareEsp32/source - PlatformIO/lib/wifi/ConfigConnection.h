#ifndef ConfigConnection_h
#define ConfigConnection_h

#include <Arduino.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SaveSettings.h"



class ConfigConnection {
    public:
        bool Begin();
        wl_status_t ConnectLAN(char* ssid, char* pwd);
        uint32_t StartConfigAP();
        bool SaveLANConfigSetting(char* AP, char* pwd);
        void SetupWebPagesRequests();
        void handleRequest (AsyncWebServerRequest *request);
        
        SaveSettings Settings;
        IPAddress myIP;
        bool AP_ConfigMode = false;
    private:
        RouterSettings router;
        const char* AP_NAME = "deLight";
};

#endif