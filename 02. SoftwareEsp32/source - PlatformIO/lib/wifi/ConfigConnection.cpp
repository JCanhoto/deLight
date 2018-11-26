#include <Arduino.h>
#include <functional>
#include <ESPmDNS.h>
#include <SPIFFS.h>
#include <FS.h>

#include "WiFi.h"
#include "ESPAsyncWebServer.h"

#include "ConfigConnection.h"
#include "SaveSettings.h"

#define NVS_DEBUG
#define WIFI_DEBUG

AsyncWebServer server(80);
 
bool ConfigConnection::Begin()
{
    // access to internal file system
    if(!SPIFFS.begin()){
        Serial.println("An Error has occurred while mounting SPIFFS");
        return false;
    }

    if (Settings.Begin()) // NVS - save settings
    {
        router = Settings.ReadRouterSettings();
        #ifdef WIFI_DEBUG
            Serial.printf("\nAP: %s - PWD: %s\n", router.SSID, router.PASSWORD);
        #endif
        
        if(ConnectLAN(router.SSID, router.PASSWORD) != WL_CONNECTED)
        {
            // did not connect after 10 retries
            #ifdef WIFI_DEBUG 
                Serial.printf("Network connection failed\nOpening AP mode: %s\nOpening AP mode\n", AP_NAME); 
            #endif
            // open the AP to allow new configuration
            
            if(StartConfigAP())
            {
                #ifdef WIFI_DEBUG 
                    Serial.println("AP Configuration mode successeful"); 
                #endif
                AP_ConfigMode = true;
            }
            else {
                #ifdef WIFI_DEBUG 
                    Serial.println("ERROR AP Configuration"); 
                #endif
                AP_ConfigMode = false;
                return false;
            }
        }
        else
        {
            AP_ConfigMode = false;
            myIP = WiFi.localIP();
            #ifdef WIFI_DEBUG 
                Serial.printf("\n\nConected success :)\n\nLocal IP:");
                Serial.print(myIP);
            #endif
        }
    }
    else return false;
                
    SetupWebPagesRequests();
    MDNS.addService("_osc","_tcp",4500);
    server.begin();

    return true;
}

bool ConfigConnection::SaveLANConfigSetting(char* AP, char* pwd)
{
    bool res=false;
    if(!Settings.Running) 
        res = Settings.Begin();

    Settings.SaveRouterSettings(AP, pwd);
    return res;
}

wl_status_t ConfigConnection::ConnectLAN(char* ssid, char* passwd)
{
    bool res = true;
    uint8_t count = 0;
    WiFi.begin(ssid, passwd);

    // Set up mDNS responder:
    res = WiFi.setHostname("delight");

    if (!(res = MDNS.begin("delight"))) {
        Serial.println("Error setting up MDNS responder!");
    }

    while (WiFi.status() != WL_CONNECTED && count < 10) {
        delay(1000);
        #ifdef WIFI_DEBUG 
            Serial.print("."); 
        #endif
        count++;
    }
    return WiFi.status();
}


uint32_t ConfigConnection::StartConfigAP()
{
#ifdef WIFI_DEBUG
  Serial.println();
  Serial.println("Configuring Setup Soft Access Point...");
#endif

    WiFi.softAP(AP_NAME); // open without the password
    WiFi.setHostname("delight");

    myIP = WiFi.softAPIP();
    // Set up mDNS responder:
    if (!MDNS.begin("delight")) {
        Serial.println("Error setting up MDNS responder!");
    }

#ifdef WIFI_DEBUG
    Serial.printf("\nAP IP address:\n");
    Serial.print(myIP);
#endif

  return 1;
}

void ConfigConnection::SetupWebPagesRequests()
{
    if(AP_ConfigMode) {
        server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
            request->send(SPIFFS, "/config.html", "text/html");
        });
    }
    else
    {
        server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
            request->send(SPIFFS, "/start.html", "text/html");
        });
    }

    server.on("/config.html", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/config.html", "text/html");
    });

    server.on("/config.html", HTTP_POST, std::bind(&ConfigConnection::handleRequest, this, std::placeholders::_1));

    server.on("/play.html", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/play.html", "text/html");
    });
    
    server.on("/w3.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/w3.css", "text/css");
    });

    server.on("/delight_small_2.png", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/delight_small_2.png", "image/png");
    });

        server.on("/delight_2.png", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/delight_2.png", "image/png");
    });
    
    server.on("/Start.jpg", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/Start.jpg", "image/png");
    });
    
    server.on("/settings.png", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/settings.png", "image/png");
    });
}

void ConfigConnection::handleRequest (AsyncWebServerRequest *request)
{
    String AP_Name;
    String AP_Password;
    int paramsNr = request->params();
    for(int i=0;i<paramsNr;i++){
 
        AsyncWebParameter* p = request->getParam(i);
        #ifdef WIFI_DEBUG
            Serial.print("Param name: ");
            Serial.println(p->name());
            Serial.print("Param value: ");
            Serial.println(p->value());
            Serial.println("------");
        #endif

        if(p->name() == "AP_Name")
            AP_Name = p->value();
        else if(p->name() == "AP_Password")
            AP_Password = p->value();
    }

    Settings.SaveRouterSettings(AP_Name, AP_Password);

    request->send(200, "text/text", "Settings Saved");

    delay(3000);
    ESP.restart();
}