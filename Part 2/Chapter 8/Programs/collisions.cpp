#include <iostream>
#include <cassert>
#include "collisions_functions.h"

using std::cout;

int main(){

  const float smallD = 1e-6;
  const float largeD = 1e-2;

  cout << "\n***Circle Data***\n\n";

  const vf circleCenter = {0,0}; const float circleRadius = 10;

  cout << "Circle center: (" << circleCenter[0] << "," << circleCenter[1] << ")\n";
  cout << "Circle radius: " << circleRadius << "\n";

  // --------------------------------------------------------------------------------

  cout << "\n***Moving Circle and Wall***\n\n";

  vf circleVector = {50,-50}; vf wallOrigin  = {100,20}; vf wallVector = {-1,-1};

  cout << "Circle displacement vector: (" <<
           circleVector[0] << "," << circleVector[1] << ")\n";
  cout << "Wall point: (" << wallOrigin[0] << "," << wallOrigin[1] << ")\n";
  cout << "Wall vector: (" << wallVector[0] << "," << wallVector[1] << ")\n";

  circle aCircle(circleCenter,circleVector,circleRadius);
  wall aWall(wallOrigin,wallVector);
  float circleWallCollisionTime = circleWallCollision(aCircle,aWall);
  cout << "Collision time: " << circleWallCollisionTime << "\n";

  assert(std::abs(circleWallCollisionTime - 0.658579) < smallD);

  // --------------------------------------------------------------------------------

  cout << "\n***Moving Point and Stationary Circle***\n\n";

  vf pointPosition = {30,6}; vf pointVector = {-78,6};

  cout << "Point position: (" << pointPosition[0] << "," << pointPosition[1] << ")\n";
  cout << "Point vector: (" << pointVector[0] << "," << pointVector[1] << ")\n";

  circle stationaryCircle(circleCenter,{0,0},circleRadius);
  point aPoint(pointPosition,pointVector);

  float pointCircleCollisionTime = pointCircleCollision(aPoint,stationaryCircle);

  cout << "Collision time: " << pointCircleCollisionTime << "\n";

  assert(std::abs(pointCircleCollisionTime - 0.304855) < smallD);

  // --------------------------------------------------------------------------------

  cout << "\n***Two Circles Straight Line***\n\n";

  straightCircle straightA(0,circleRadius,60);
  straightCircle straightB(100,circleRadius,-50);
  float straightDistanceToTravel = 100 - 2 * circleRadius;

  cout << "Circle linear positions: " <<
    straightA.position << ", " << straightB.position << '\n';
  cout << "Circle radiuses: " << straightA.radius << '\n';
  cout << "Circle speeds: " <<
    straightA.speed << ", " << straightB.speed << '\n';

  float straightCircleTime = circleCircleStraightCollision(straightA,straightB);

  cout << "Collision time: " << straightCircleTime << '\n';

  assert((straightA.speed - straightB.speed) * straightCircleTime ==
          straightDistanceToTravel);

  // --------------------------------------------------------------------------------

  cout << "\n***Circle Circle Collision***\n\n";

  vf circleAngleSpeed = {-100,60}; vf circleAnglePosition = {60,-20};
  float circleAngleSmallRadius = 5;

  circle circleAngleA(circleCenter,{0,0},circleRadius);
  circle circleAngleB(circleAnglePosition,circleAngleSpeed,circleAngleSmallRadius);

  cout << "Stationary circle position: (" << circleAngleA.position[0] <<
  ", " << circleAngleA.position[1] << "), radius: " << circleRadius << '\n';

  cout << "Moving circle position: (" <<
    circleAngleB.position[0] << ", " <<
    circleAngleB.position[1] <<
    " ), radius: " << circleAngleSmallRadius << ", " <<
    "displacement: (" << circleAngleB.displacement[0] << ", " <<
                         circleAngleB.displacement[1] << ")\n";

  float circleCircleTime = circleCircleCollision(circleAngleA,circleAngleB);

  cout << "Collision time: " << circleCircleTime << '\n';

  vf newPosition = add(circleAnglePosition,mul(circleAngleSpeed,circleCircleTime));

  assert(std::abs(newPosition[0] - 12.25) < largeD &&
         std::abs(newPosition[1] - 8.65) < largeD);

  // --------------------------------------------------------------------------------

  cout << "\n***Circle Inside Circle***\n\n";

  circle innerCircle(circleCenter,{0,-600},circleRadius);
  circle outerCircle({40,-40},{0,0},80);

  cout << "Inner circle position: (" <<
    innerCircle.position[0] << ',' <<
    innerCircle.position[1] <<
    "), radius: " << innerCircle.radius <<
    ", velocity: (" << innerCircle.displacement[0] << ", " <<
                       innerCircle.displacement[1] << ")\n";

  cout << "Outer circle position: (" <<
    outerCircle.position[0] << ", " <<
    outerCircle.position[1] <<
    "), radius: " << outerCircle.radius << '\n';

  float innerCollisionTime = circleCircleInnerCollision(innerCircle,outerCircle);

  cout << "Collision time: " << innerCollisionTime << '\n';

  vf newInnerPosition =
    add(innerCircle.position,
        mul(innerCircle.displacement,
            innerCollisionTime));

  assert(newInnerPosition[0] == 0 &&
         std::abs(newInnerPosition[1] - -97.44) < largeD);

  // --------------------------------------------------------------------------------

  cout << "\n***Point Inside Rectangle***\n\n";

  vf rectangleCenter = {0,0}; vf longSide = {4,-4}; vf shortSide = {2,2};
  vf pointInsideR = {2,-5.5};

  bool isPointInside =
    pointInsideRectangle(pointInsideR,rectangleCenter,longSide,shortSide);

  cout << "Point (" <<
    pointInsideR[0] << ", " << pointInsideR[1] << ") is " <<
    (isPointInside ? "inside" : "outside") << " rectangle\n";

  assert(isPointInside);

  // --------------------------------------------------------------------------------

  cout << "\n***Point On Rectangle***\n\n";

  vf pOnRec = {5,-3};

  bool isPointOnRec =
    pointOnRectangle(pOnRec,rectangleCenter,longSide,shortSide);

  if (isPointOnRec){
    cout << "Point (" <<
      pOnRec[0] << ", " << pOnRec[1] <<
      ") is on the perimeter of the rectangle\n\n";
  }

  assert(isPointOnRec);

  // --------------------------------------------------------------------------------

  cout << "\n***Point Rectangle Intersection***\n\n";

  const point somePoint({15,2},{-3.75,0.5});
  const rectangle someRectangle(rectangleCenter,shortSide,longSide);

  cout << "Test:\n";
  cout << pointRectangleIntersection(somePoint, someRectangle);

  // --------------------------------------------------------------------------------

  cout << "\n***Two Aligned Rectangles***\n\n";

  // rectangleRectangleCollisionStraight(const rectangle & rec1,const rectangle & rec2)

  // --------------------------------------------------------------------------------

  cout << "\n***Two Rectangles At Angle***\n\n";

  // --------------------------------------------------------------------------------

  cout << "\n***Ellipse Drawing***\n\n";

  const vf foc1 = {-4,0}; const vf foc2 = {4,0};
  const float ellipseResolution = 20;
  const float internalDiameter = 10;

  const std::vector<vf> ellipsePoints =
    drawEllipseByFoci(foc1,foc2,internalDiameter,ellipseResolution);

  for (const auto & p : ellipsePoints){
    cout << "Point: " << p[0] << ", " << p[1] << "\n";
  }

  // --------------------------------------------------------------------------------

  cout << "Part 2 - Chapter 8 - Collisions\n";

}
