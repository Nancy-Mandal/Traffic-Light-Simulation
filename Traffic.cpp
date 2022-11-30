#include <Traffic.h>
#include <Adafruit_MCP23017.h>
#include <Arduino.h>

TrafficPole::TrafficPole (int _id, int _green, int _orange, int _red, int _vehicleDemand)
{
  id = _id;
  green = _green;
  orange = _orange;
  red = _red;
  // pedestrian = _pedestrian;
  vehicleDemand = _vehicleDemand;
}

void TrafficPole::setGreen(int Time)
{
  priority = 0;
  mcp.digitalWrite(red, LOW);
  mcp.digitalWrite(orange, LOW);
  mcp.digitalWrite(green, HIGH);
  state = 2;
  cutoff = Time;
}

void TrafficPole::turnOffGreen()
{
  myTime = 0;
  mcp.digitalWrite(green, LOW);
  mcp.digitalWrite(red, LOW);
  mcp.digitalWrite(orange, HIGH);
  state = 1;
  cutoff = orangeWaitTime;
}

void TrafficPole::turnOnRed()
{
  myTime = 0;
  mcp.digitalWrite(orange, LOW);
  mcp.digitalWrite(green, LOW);
  mcp.digitalWrite(red, HIGH);
  state = 0;
  cutoff = 0;
}

void TrafficPole::Update(bool &isGreen)
{
  Serial.print(-id);
  Serial.println(priority);
  if (state == 2)
  {
    myTime += fixedDeltaTime;
    if (myTime >= cutoff)
    {
      TrafficPole::turnOffGreen();
    }
  }
  else if (state == 1)
  {
    myTime += fixedDeltaTime;
    if (myTime >= cutoff)
    {
      TrafficPole::turnOnRed();
      emergency = false;
      isGreen = false;
    }
  }
  else
  {
    priority += map(analogRead(vehicleDemand), 0, 1023, 0, wVehicle) + wWaitTime + emergency * wEmergency;
  }
}

void TrafficPole::begin(Adafruit_MCP23017 &_mcp)
{
  mcp = _mcp;
  mcp.digitalWrite(red, HIGH);
  // mcp.digitalWrite(12, HIGH);
}
  
