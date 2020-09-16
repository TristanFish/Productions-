#include "RigidBody3D.h"
#include "math/VMATH.h"
#include "core/Timer.h"


RigidBody3D::RigidBody3D(): mass(1.0f), vel(MATH::Vec3()), accel(MATH::Vec3()), linearDrag(0.0f), rotInertia(0.0f),
										  zAngle(0.0f), angularVel(0.0f), angularAcc(0.0f), angularDrag(0.95f)
{

}

RigidBody3D::~RigidBody3D()
{
	pos = nullptr;
}

void RigidBody3D::Init(GameObject *g)
{
	gameobject = g;
	pos = &g->transform.pos;
	setColliderSize(g->transform.GetScale().x);

	mass = 1.0f;
	vel = MATH::Vec3();
	accel = MATH::Vec3();

	rotInertia = 1.0f;
	zAngle = 0.0f;
	angularVel = 0.0f;
	angularAcc = 0.0f;
}

void RigidBody3D::Update(const float deltaTime)
{
	vel += accel * deltaTime;
	*pos += vel * deltaTime + 0.5f * accel * deltaTime * deltaTime;


	angularVel += angularAcc * deltaTime;
	angularVel *= angularDrag;
	zAngle += angularVel * deltaTime + 0.5f * angularAcc * deltaTime * deltaTime;

	gameobject->SetRotation(MATH::Vec3(0.0f, 0.0f, zAngle));

}

void RigidBody3D::Render() const
{
	//RigidBody doesn't do any rendering
}

void RigidBody3D::HandleEvents(const SDL_Event& event)
{
	//RigidBody doesn't do any event listening
}

void RigidBody3D::ApplyImpulseForce(const MATH::Vec3& force)
{
	vel += (force / mass) * Timer::GetScaledDeltaTime();
}

void RigidBody3D::SetConstantForce(const MATH::Vec3& force)
{
	accel = force / mass;
}
void RigidBody3D::ApplyConstantForce(const MATH::Vec3& force)
{
	accel += force / mass;
}

void RigidBody3D::ApplyImpulseTorque(const float torque)
{
	angularVel += (torque / rotInertia) * Timer::GetScaledDeltaTime();
}

void RigidBody3D::ApplyConstantTorque(const float torque)
{
	angularAcc = torque / mass;
}