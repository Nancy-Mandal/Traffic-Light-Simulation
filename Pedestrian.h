#pragma once
#include <Adafruit_MCP23017.h>

class Pedestrian
{
  public :
  Adafruit_MCP23017 mcp;
  int id, green, orange, red, pedestrianDemand1, pedestrianDemand2, pedestrianDemand3, pedestrianDemand4;
  
  int wPedestrians {3}, wWaitTime {0}, fixedDeltaTime{300};	                                                         // Stores pin ids

  int state {0};												                                                     // State = 0(red), 1(orange), 2(green) 

  int priority {0};

  int orangeWaitTime {1000};

  int myTime {0};
  int cutoff {0};

  bool emergency {false};
  // bool pedestrian;

  Pedestrian (int _id, int _green, int _orange, int _red, int _pedestrianDemand1, int _pedestrianDemand2, int _pedestrianDemand3, int _pedestrianDemand4);

  void begin(Adafruit_MCP23017 &_mcp);

  void setGreen(int Time);

  void turnOffGreen();

  void turnOnRed();

  void Update(bool &isGreen);
};