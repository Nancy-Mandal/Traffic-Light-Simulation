#pragma once
#include <Adafruit_MCP23017.h>

class TrafficPole
{
  public :
  Adafruit_MCP23017 mcp;
  int id, green, orange, red, vehicleDemand; 
  
  int wVehicle {30}, wWaitTime {1}, wEmergency{50}, fixedDeltaTime{300};                                                       // Stores pin ids

  int state {0};                                                                                            // State = 0(red), 1(orange), 2(green) 

  int priority {0};

  int orangeWaitTime {400};

  int myTime {0};
  int cutoff {0};

  bool emergency {false};
  // bool pedestrian;

  TrafficPole (int _id, int _green, int _orange, int _red, int _vehicleDemand);

  void begin(Adafruit_MCP23017 &_mcp);

  void setGreen(int Time);

  void turnOffGreen();

  void turnOnRed();

  void Update(bool &isGreen);
};