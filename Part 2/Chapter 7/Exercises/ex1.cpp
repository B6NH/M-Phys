#include <iostream>
#include <array>
#include <cmath>
#include "../Programs/energy_functions.h"

std::pair<std::array<float,2>,float> javelin(float throwAngle,
                                             float throwSpeed, float time);

using std::cout;
using std::endl;


int main(){
  
  const float initialAngle { 0.8 };
  const float initialSpeed { 25 };
  const float step { 0.5 };
  const float seconds { 3.5 };
  
  for(float i=0;i<=seconds;i+=step){
    
    auto result = javelin(initialAngle,initialSpeed,i);
    
    cout << "After " << i << " seconds:\n";
    
    cout << "  Position: ";
    cout << result.first.at(0) << ", ";
    cout << result.first.at(1) << endl;
    
    cout << "  Angle: ";
    cout << result.second << endl;
    
  }
  
  std::cout << "Part 2 - Chapter 7 - Exercise 1\n";
  
}

std::pair<std::array<float,2>,float> javelin(float throwAngle,
                                             float throwSpeed, float time){
    
  const float acceleration = -10;
  
  const float vertical = sin(throwAngle) * throwSpeed;
  const float horizontal = cos(throwAngle) * throwSpeed;
  
  const float javelinX = time * horizontal;
  const float javelinY = distanceAcceleration(vertical,acceleration,time);

  const float currentVertical = vertical + acceleration * time;
  const float currentAngle = atan2(currentVertical,horizontal);
  
  return std::make_pair(std::array<float,2>({ javelinX, javelinY }), currentAngle);
  
}
