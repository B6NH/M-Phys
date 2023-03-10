#include "geometry_functions.h"
#include <cmath>
#include <iostream>

using std::cout;

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
    float result = acos((sqr(m1)+sqr(m2)-sqr(m3))/(2*m1*m2));
    return a.first < b.first ? -result : result;
  }

}

void showTriangle(const triangle & vertices){
  for(int i=0;i<3;i++){
    cout << "(" << vertices.at(i).first << "," << vertices.at(i).second << ")";
  }
  cout << '\n';
}

triangle rotateFollow(const triangle & vertices,const point & target){

  // Find centrum of a triangle
  const point centrum = triangleCenter(vertices);

  mvector centerToFront = createVector(centrum,vertices.at(0));
  mvector centerToTarget = createVector(centrum,target);

  const float angle = calculateAngle(centerToFront,centerToTarget);

  triangle result;
  for(int i=0;i<3;i++){
    result.at(i) = rotatePoint(vertices.at(i),centrum,angle);
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

  float newX = sqrt(sqr(tX)+sqr(tY)) * cos(angle+atan2(tY,tX)) + s;
  float newY = sqrt(sqr(tX)+sqr(tY)) * sin(angle+atan2(tY,tX)) + t;

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
  float result;
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
    result = atan(y/x);
  }else{
    result = degToRad(deg);
  }
  return result;
}

triangleInfo solveTriangle(const triangleInfo & data){

  triangleInfo result;
  int sides = countSides(data); int angles = countAngles(data);

  // Complete solution

  // 3 sides
  if(3==sides){
    return solve3Sides(data);

  // 2 angles and 1 side
  }else if(sides>=1&&angles>=2){
    return solve2Angles1Side(data,angles);

  // 2 sides and angle between them
  }else if(hasAngleBetween(data)){
    return angleBetween2Sides(data);

  // Right-angled triangle and 2 sides(hypotenuse and 1 side)
  // If angle between a and b is 90, sides are a-c or b-c (a-b in previous case for any angle)
  }else if(isRightAngled(data)){
    return rightAngle2Sides(data);

  // Incomplete solution
  }else{

    if(2==angles){
      result = data; thirdAngle(result);
    }

  }

  return result;

}

int countData(const triangleInfo & data, int from){
  int count = 0; int end = from + 3; // read 3 elements
  for(int i=from;i<end;i++){ if(data.at(i)>=0){ count++; } }
  return count;
}

int countSides(const triangleInfo & data){
  return countData(data,0);
}

int countAngles(const triangleInfo & data){
  return countData(data,3);
}

float cosineR(float a, float b, float c){
  return radToDeg(acos((sqr(b)+sqr(c)-sqr(a))/(2*b*c)));
}

bool compareAprox(const triangleInfo & a, const triangleInfo & b){
  for(int i=0;i<a.size();i++){
    if(std::abs(a.at(i)-b.at(i))>0.9){
      return false;
    }
  }
  return true;
}

bool hasAngleBetween(const triangleInfo & data){
  float a, b, c, d, e, f;
  a = data.at(0); b = data.at(1); c = data.at(2);
  d = data.at(3); e = data.at(4); f = data.at(5);
  return (a!=-1&&b!=-1&&f!=-1)||
         (b!=-1&&c!=-1&&d!=-1)||
         (c!=-1&&a!=-1&&e!=-1);
}

bool isRightAngled(const triangleInfo & data){
  for(int i=3;i<6;i++){
    if(data.at(i)==90){ return true; }
  }
  return false;
}

float calcSide(float b, float c, float alfa){
    return sqrt(sqr(b) + sqr(c) - 2 * b * c * cos(degToRad(alfa)));
}

float pythagore(float side, float hypotenuse){
  return sqrt(sqr(hypotenuse)-(sqr(side)));
}

triangleInfo solve3Sides(triangleInfo data){

  float a, b, c;
  a = data.at(0); b = data.at(1); c = data.at(2);

  // Calculate angles with cosine rule
  data.at(3) = cosineR(a,b,c);
  data.at(4) = cosineR(b,c,a);
  data.at(5) = cosineR(c,a,b);

  return data;

}

void thirdAngle(triangleInfo & data){

  float sum = 0; int angleIndex;

  // sum of 2 angles
  for(int i=3;i<6;i++){
    if(data.at(i)>=0){
      sum += data.at(i);
    }else{
      angleIndex = i;
    }
  }

  data.at(angleIndex) = 180 - sum;

}

triangleInfo solve2Angles1Side(triangleInfo data, int angles){

  // Find 3rd angle
  if(2==angles){ thirdAngle(data); }

  // 3 angles and at least 1 side are now known

  // Find index of known side
  int sideIndex = 0;
  while(data.at(sideIndex)<0){ sideIndex++; }

  // Calculate proportion. Add 3 to sideIndex to find corresponding angle
  const float proportion =
    data.at(sideIndex) / sin(degToRad(data.at(sideIndex+3)));

  // Calculate remaining sides from sine rule
  // a = sin(alfa) * (c/sin(gamma))
  for(int i=0;i<3;i++){
    if(i!=sideIndex){
      data.at(i) =
        sin(degToRad(data.at(i+3))) * proportion;
    }
  }

  return data;
}

triangleInfo angleBetween2Sides(triangleInfo data){

  float a, b, c;
  a = data.at(0); b = data.at(1); c = data.at(2);

  // Find index of known angle
  int angleIndex = 3;
  while(data.at(angleIndex)==-1){ angleIndex++; }

  // Find remaining side
  switch(angleIndex){

    case 3:
      data.at(0) = calcSide(b,c,data.at(angleIndex));
      break;

    case 4:
      data.at(1) = calcSide(a,c,data.at(angleIndex));
      break;

    case 5:
      data.at(2) = calcSide(a,b,data.at(angleIndex));
      break;
  }

  // When 3 sides are known call function again
  return solveTriangle(data);

}

// 2 sides = 1 side + hypotenuse
triangleInfo rightAngle2Sides(triangleInfo data){

  float a, b, c;
  a = data.at(0); b = data.at(1); c = data.at(2);

  // Find index of right angle
  int index = 3;
  while(data.at(index)!=90){ index++; }


  // Find remaining side
  switch(index){

    // Right angle (2-3)
    // Check pairs (1-2, 1-3)
    case 3:

      if(a!=-1){
        if(b!=-1){
          data.at(2) = pythagore(b,a);
        }else if(c!=-1){
          data.at(1) = pythagore(c,a);
        }
      }

      break;

    // Right angle (1-3)
    // Check pairs (1-2, 2-3)
    case 4:

      if(b!=-1){
        if(a!=-1){
          data.at(2) = pythagore(a,b);
        }else if(c!=-1){
          data.at(0) = pythagore(c,b);
        }
      }

      break;

    // Right angle (1-2)
    // Check pairs (1-3, 2-3)
    case 5:

      if(c!=-1){
        if(a!=-1){
          data.at(1) = pythagore(a,c);
        }else if(b!=-1){
          data.at(0) = pythagore(b,c);
        }
      }

      break;

  }

  // When 3 sides are known call function again
  return solveTriangle(data);
}
