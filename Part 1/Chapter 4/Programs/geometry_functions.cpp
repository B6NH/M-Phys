#include "geometry_functions.h"
#include <cmath>
#include <iostream>

using std::cout; using std::endl;

point pointBetween(const point & a,const point & b, float distance){
  float totalDistance = calculateDistance(a,b);
  float ratio = distance / totalDistance;
  float x = a.first + ratio * (b.first - a.first);
  float y = a.second + ratio * (b.second - a.second);
  return std::make_pair(x,y);
}

float calculateDistance(const point & a, const point & b){
  return magnitude(createVector(a,b));
}

// Angle between vectors
float calculateAngle(const mvector & a, const mvector & b){

  mvector c = std::make_pair(b.first-a.first,b.second-a.second);
  float m1 = magnitude(a); float m2 = magnitude(b); float m3 = magnitude(c);

  // Error(if m1 or m2 == 0) or vectors are equal
  if(m1==0||m2==0||m3==0){
    return 0;
  }else{
    return acos((sqr(m1)+sqr(m2)-sqr(m3))/(2*m1*m2));
  }

}

triangle rotateFollow(const triangle & vertices,const point & target){

  // Find centrum of a triangle
  const point centrum = triangleCenter(vertices);

  mvector centerToFront = createVector(centrum,vertices.at(0));
  mvector centerToTarget = createVector(centrum,target);

  const float angle = calculateAngle(centerToFront,centerToTarget);

  triangle result;
  for(int i=0;i<3;i++){
    result.at(i) = rotatePoint(vertices.at(i),centrum,-angle);
  }

  return result;

}

float magnitude(const mvector & vec){
  return sqrt(sqr(vec.first) + sqr(vec.second));
}

mvector createVector(const point & a, const point & b){
  return std::make_pair(b.first-a.first,b.second-a.second);
}

point triangleCenter(const triangle & vertices){
  float x = (vertices.at(0).first + vertices.at(1).first + vertices.at(2).first) / 3;
  float y = (vertices.at(0).second + vertices.at(1).second + vertices.at(2).second) / 3;
  return std::make_pair(x,y);
}

point rotatePoint(const point & pnt, const point & around, float angle){

  float x = pnt.first; float y = pnt.second;
  float s = around.first; float t = around.second;
  float tX = x - s; float tY = y - t; // Translate point

  float newX = sqrt(sqr(tX)+sqr(tY)) * cos(-angle-atan2(tY,tX)) + s;
  float newY = sqrt(sqr(tX)+sqr(tY)) * sin(angle-atan2(tY,tX)) + t;

  return std::make_pair(newX,newY);

}

float sqr(float x){
  return pow(x,2);
}

float degToRad(float deg){
  return deg * 0.0174532925;
}

float radToDeg(float rad){
  return rad * 57.2957795;
}

float mytan2(float y,float x){
  float deg = 1;
  if(x==0&&y<0){
    deg = 90;
  }else if(x==0&&y>=0){
    deg = -90;
  }else if(y==0&&x<0){
    deg = 0;
  }else if(y==0&&x>=0){
    deg = -180;
  }else if(deg==1){
    return atan(y/x);
  }else{
    return degToRad(deg);
  }
}

triangleInfo solveTriangle(const triangleInfo & data){

  int sides = countSides(data); int angles = countAngles(data);

  if(3==sides){

    int a = data.at(0); int b = data.at(1); int c = data.at(2);
    triangleInfo result = data;

    // Calculate angles with sine rule
    result.at(3) = radToDeg(sineR(a,b,c));
    result.at(4) = radToDeg(sineR(b,c,a));
    result.at(5) = radToDeg(sineR(c,a,b));

    cout << result.at(3) << endl;
    cout << result.at(4) << endl;
    cout << result.at(5) << endl;

    return result;
  }

}

int countData(const triangleInfo & data, int from){
  int count = 0; int end = from + 3; // read 3 elements
  for(int i=from;i<end;i++){
    if(data.at(i)>=0){ count++; }
  }
  return count;
}

int countSides(const triangleInfo & data){
  return countData(data,0);
}

int countAngles(const triangleInfo & data){
  return countData(data,3);
}

float sineR(float a, float b, float c){
  return acos((pow(b,2)+pow(c,2)-pow(a,2))/(2*b*c));
}

bool compareAprox(const triangleInfo & a, const triangleInfo & b){
  for(int i=0;i<a.size();i++){
    if(std::abs(a.at(i)-b.at(i))>0.01){
      return false;
    }
  }
  return true;
}
