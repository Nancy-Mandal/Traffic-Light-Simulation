#include <Adafruit_MCP23017.h>
#include <Traffic.h>
#include <Pedestrian.h>

int fixedDeltaTime {300};

int wVehicle {20}, wEmergency {50}, wWaitTime {1}, wPedestrian {2};

int iterationvariable {0};

bool isGreen {false};

int greenTime {0};

int maxWaitTime{10000}, maxPedWaitTime{10000};

int green_id {0};

// int emer1 {8}, emer2 {9}, emer3 {10}, emer4 {11};
int emer1 {4}, emer2 {5}, emer3 {8}, emer4 {9};

Adafruit_MCP23017 mcp;

/*
TrafficPole lane1(1, 0, 1, 2, A0);
TrafficPole lane2(2, 3, 4, 5, A1);
TrafficPole lane3(3, 6, 7, 8, A2);
TrafficPole lane4(4, 9, 10, 11, A3);
*/

TrafficPole lane1(1, 8, 9, 10, A3);
TrafficPole lane2(2, 5, 6, 7, A2);
TrafficPole lane3(3, 2, 1, 0, A1);
TrafficPole lane4(4, 13, 14, 15, A0);

// Pedestrian pedestrian(5, 12, 13, 14, 4, 5, 6, 7);
Pedestrian pedestrian(5, 12, 11, 3, 2, 3, 6, 7);

void setup()
{
  Serial.begin(9600);
  mcp.begin();
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  for (int i = 0; i < 16; i ++)
    mcp.pinMode(i, OUTPUT);

/*
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
*/

  // pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);

  pinMode(7, INPUT);

  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);

  lane1.begin(mcp);
  lane2.begin(mcp);
  lane3.begin(mcp);
  lane4.begin(mcp);

  pedestrian.begin(mcp);
}

void loop()
{  
  if (digitalRead(emer1) == HIGH && !lane1.emergency && !lane2.emergency && !lane3.emergency && !lane4.emergency)
    {
      lane1.emergency = true;
      if (lane1.state != 2)
      {
        if (green_id == 2)
          lane2.turnOnRed();
        if (green_id == 3)
          lane3.turnOnRed();
        if (green_id == 4)
          lane4.turnOnRed();
        if (green_id == 5)
          pedestrian.turnOnRed();
        lane1.setGreen(5000);
        Serial.println("Emergency Green");
        isGreen = true;
      }
      else
      {
        lane1.cutoff = lane1.myTime + 5000;
        lane1.emergency = true;
      }
    }
  if (digitalRead(emer2) == HIGH && !lane1.emergency && !lane2.emergency && !lane3.emergency && !lane4.emergency)
    {
      lane2.emergency = true;
      if (lane2.state != 2)
      {
        if (green_id == 1)
          lane1.turnOnRed();
        if (green_id == 3)
          lane3.turnOnRed();
        if (green_id == 4)
          lane4.turnOnRed();
        if (green_id == 5)
          pedestrian.turnOnRed();
        lane2.setGreen(5000);
        isGreen = true;
      }
      else
      {
        lane2.cutoff = lane2.myTime + 5000;
        lane2.emergency = true;
      }
    }
  if (digitalRead(emer3) == HIGH && !lane1.emergency && !lane2.emergency && !lane3.emergency && !lane4.emergency)
    {
      lane3.emergency = true;
      if (lane3.state != 2)
      {
        if (green_id == 2)
          lane2.turnOnRed();
        if (green_id == 1)
          lane1.turnOnRed();
        if (green_id == 4)
          lane4.turnOnRed();
        if (green_id == 5)
          pedestrian.turnOnRed();
        lane3.setGreen(5000);
        isGreen = true;
      }
      else
      {
        lane3.cutoff = lane3.myTime + 5000;
        lane3.emergency = true;
      }
    }
  if (digitalRead(emer4) == HIGH && !lane1.emergency && !lane2.emergency && !lane3.emergency && !lane4.emergency)
    {
      lane4.emergency = true;
      if (lane4.state != 2)
      {
        if (green_id == 2)
          lane2.turnOnRed();
        if (green_id == 3)
          lane3.turnOnRed();
        if (green_id == 1)
          lane1.turnOnRed();
        if (green_id == 5)
          pedestrian.turnOnRed();
        lane4.setGreen(5000);
        Serial.println("Emergency Green");
        isGreen = true;
      }
      else
      {
        lane4.cutoff = lane4.myTime + 5000;
        lane4.emergency = true;
        Serial.println("Emergency Cutoff Increased");
      }
    } 
  
  if (!isGreen)
  {
    int maxVal = lane1.priority;
    int lane = 1;
    if (maxVal < lane2.priority)
    {
      lane = 2;
      maxVal = lane2.priority;
    }
    if (maxVal < lane3.priority)
    {
      lane = 3;
      maxVal = lane3.priority;
    }
    if (maxVal < lane4.priority)
    {
      lane = 4;
      maxVal = lane4.priority;
    }
    if (maxVal < pedestrian.priority)
    {
      lane = 5;
      maxVal = pedestrian.priority;
    }
    
    if (lane < 5) greenTime = map(maxVal, 0, lane1.priority + lane2.priority + lane3.priority + lane4.priority, maxWaitTime / 2, maxWaitTime);
    else greenTime = maxPedWaitTime;
    
    Serial.println(greenTime);

    green_id = lane;

/*
    if (lane1.priority == 0)
      digitalWrite(3, HIGH);
    else
      digitalWrite(3, LOW);
    if (lane2.priority == 0)
      digitalWrite(4, HIGH);
    else
      digitalWrite(4, LOW);
    if (lane3.priority == 0)
      digitalWrite(5, HIGH);
    else
      digitalWrite(5, LOW);
    if (lane4.priority == 0)
      digitalWrite(6, HIGH);
    else
      digitalWrite(6, LOW);
*/
    
    switch (lane)
    {
      case 1 :
        lane1.setGreen(greenTime);
        break;
      case 2 :
        lane2.setGreen(greenTime);
        break;
      case 3 :
        lane3.setGreen(greenTime);
        break;
      case 4 :
        lane4.setGreen(greenTime);
        break;
      case 5 :
        pedestrian.setGreen(greenTime);
        break;
    }
    isGreen = true;
  }
  
  lane1.Update(isGreen);
  lane2.Update(isGreen);
  lane3.Update(isGreen);
  lane4.Update(isGreen);
  pedestrian.Update(isGreen);

  delay(fixedDeltaTime / 6 * 5);

  /*
  lane1.mcp.digitalWrite(lane1.red, HIGH);
  lane2.mcp.digitalWrite(lane2.orange, HIGH);
  lane3.mcp.digitalWrite(lane3.green, HIGH);
  lane4.mcp.digitalWrite(lane4.red, HIGH);
  */
}
