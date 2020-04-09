#ifndef PHYSICS_H
#define PHYSICS_H

#include "math/VMath.h"
#include "MMath.h"
#include "components/RigidBodyComponent.h"

class Physics
{
private:

	static bool CircleCircleDetect(RigidBodyComponent& rb1, RigidBodyComponent& rb2);
	static bool CircleBoxDetect(RigidBodyComponent& rb1, RigidBodyComponent& rb2);
	static bool BoxBoxDetect(RigidBodyComponent& rb1, RigidBodyComponent& rb2);

	static void CircleCircleResolve(RigidBodyComponent& rb1, RigidBodyComponent& rb2);
	static void CircleBoxResolve(RigidBodyComponent& rb1, RigidBodyComponent& rb2);
	static void BoxBoxResolve(RigidBodyComponent& rb1, RigidBodyComponent& rb2);

	
public:
	static bool DetectCollision(RigidBodyComponent& rb1, RigidBodyComponent& rb2);

};

#endif