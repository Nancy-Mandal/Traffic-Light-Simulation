#include <Pedestrian.h>
#include <Adafruit_MCP23017.h>
#include <Arduino.h>

Pedestrian::Pedestrian(int _id, int _green, int _orange, int _red, int _pedestrianDemand1, int _pedestrianDemand2, int _pedestrianDemand3, int _pedestrianDemand4)
{
    id = _id;
    green = _green;
    orange = _orange;
    red = _red;
    // pedestrian = _pedestrian;
    pedestrianDemand1 = _pedestrianDemand1;
    pedestrianDemand2 = _pedestrianDemand2;
    pedestrianDemand3 = _pedestrianDemand3;
    pedestrianDemand4 = _pedestrianDemand4;
}

void Pedestrian::setGreen(int Time)
{
    priority = 0;
    mcp.digitalWrite(red, LOW);
    mcp.digitalWrite(orange, LOW);
    mcp.digitalWrite(green, HIGH);
    state = 2;
    cutoff = Time;
}

void Pedestrian::turnOffGreen()
{
    myTime = 0;
    mcp.digitalWrite(green, LOW);
    mcp.digitalWrite(red, LOW);
    mcp.digitalWrite(orange, HIGH);
    state = 1;
    cutoff = orangeWaitTime;
}

void Pedestrian::turnOnRed()
{
    myTime = 0;
    mcp.digitalWrite(orange, LOW);
    mcp.digitalWrite(green, LOW);
    mcp.digitalWrite(red, HIGH);
    state = 0;
    cutoff = 0;
}

void Pedestrian::Update(bool& isGreen)
{
    Serial.println(priority);
    if (state == 2)
    {
        myTime += fixedDeltaTime;
        if (myTime >= cutoff)
        {
            Pedestrian::turnOffGreen();
            emergency = false;
        }
    }
    else if (state == 1)
    {
        myTime += fixedDeltaTime;
        if (myTime >= cutoff)
        {
            Pedestrian::turnOnRed();
            isGreen = false;
        }
    }
    else
    {
        priority += (digitalRead(pedestrianDemand1) + digitalRead(pedestrianDemand2) + digitalRead(pedestrianDemand3) + digitalRead(pedestrianDemand4)) * wPedestrians + wWaitTime;
    }
}

void Pedestrian::begin(Adafruit_MCP23017& _mcp)
{
    mcp = _mcp;
    mcp.digitalWrite(red, HIGH);
    // mcp.digitalWrite(12, HIGH);
}

