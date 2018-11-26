/*
  SaveSettings.h - Library for saving config data into eeprom.
  Created by Joaquim Canhoto, Set 2018
  Released into the public domain.
*/

#ifndef SaveSettings_h
#define SaveSettings_h

//#define FIRST_TIME

struct RouterSettings {
  char* SSID;
  char* PASSWORD;
};

class SaveSettings {
public:
  bool Begin();
  void ResetAllData();
  void SaveRouterSettings(String AP_Name, String password);
  RouterSettings ReadRouterSettings();
  bool Running = false;
private:

};



#endif