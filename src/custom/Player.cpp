#include "Player.h"
#include "core/Timer.h"
#include "custom/Camera.h"
#include "tiles/Tile.h"

 float Player::jetPower = 0.0f;

Player::Player(const char* n, const MATH::Vec3& pos)
{

	name = n;
	transform = Transform(pos);

	//Always initialize the components that you've inherited with your current gameobject
	//this allows the components to access the transform of of your gameobject
	RigidBodyComponent::Init(this);
	SpriteComponent::Init(this);

	RigidBodyComponent::setColliderShape(Collider::shape::Circle);
	RigidBodyComponent::setColliderSize(2.0f);
	RigidBodyComponent::SetConstantForce(MATH::Vec3(0.0f, -6.0f, 0.0f));
}

Player::~Player()
{
	
}

void Player::Update(const float deltaTime)
{
	//Always update your inherited components and transform
	transform.Update(deltaTime);
	RigidBodyComponent::Update(deltaTime);
	SpriteComponent::Update(deltaTime);
	Camera::getInstance()->setPosition(VMath::lerp(Camera::getInstance()->getPosition(), transform.GetPosition(), 0.05f));
}

void Player::Render() const
{
	//Render your inherited components
	RigidBodyComponent::Render();
	SpriteComponent::Render();
}

void Player::HandleEvents(const SDL_Event& event)
{
	if (event.type == SDL_EventType::SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_w) {

			if (jetPower > 0.0)
			{
				jetPower -= 0.3;
				RigidBodyComponent::ApplyImpulseForce(MATH::MMath::rotate(transform.GetRotation().z, MATH::Vec3(0.0f, 0.0f, 1.0f))
					* MATH::Vec3(0.0f, 5.0f, 0.0f) * 2.0f);
				RigidBodyComponent::SetIsGrounded(false);
			}
		}

		if (event.key.keysym.sym == SDLK_q)
		{
			if (!RigidBodyComponent::GetIsGrounded())
			{
				RigidBodyComponent::ApplyImpulseTorque(5.0f);
				std::cout << RigidBodyComponent::GetAngVelocity() << std::endl;
			}
		}
		if (event.key.keysym.sym == SDLK_e)
		{
			if (!RigidBodyComponent::GetIsGrounded())
			{
				RigidBodyComponent::ApplyImpulseTorque(-5.0f);
				std::cout << RigidBodyComponent::GetAngVelocity() << std::endl;
			}
		}
		if (event.key.keysym.sym == SDLK_a)
		{
			if (RigidBodyComponent::GetIsGrounded())
			{
				SetScale(Vec3(-2, transform.scale.y, transform.scale.z));
				RigidBodyComponent::SetVelocity(Vec3(-8.0, 0.0, 0.0));
			}
		}
		if (event.key.keysym.sym == SDLK_d)
		{
			if (RigidBodyComponent::GetIsGrounded())
			{
				SetScale(Vec3(2, transform.scale.y, transform.scale.z));
				RigidBodyComponent::SetVelocity(Vec3(8.0, 0.0, 0.0));
			}
		}
		if (event.key.keysym.sym == SDLK_SPACE)
		{
			if (RigidBodyComponent::GetIsGrounded())
			{
				RigidBodyComponent::SetVelocity(Vec3(RigidBodyComponent::GetVelocity().x / 2, 5.0, 0.0));
				RigidBodyComponent::SetIsGrounded(false);
			}
		}
	}

	if (event.type == SDL_EventType::SDL_KEYUP)
	{
		if (event.key.keysym.sym == SDLK_a)
		{
			if (RigidBodyComponent::GetIsGrounded())
			{
				RigidBodyComponent::SetVelocity(Vec3(0.0, 0.0, 0.0));
				RigidBodyComponent::SetAccel(Vec3(0.0, 0.0, 0.0));
			}
		}
		if (event.key.keysym.sym == SDLK_d)
		{
			if (RigidBodyComponent::GetIsGrounded())
			{
				RigidBodyComponent::SetVelocity(Vec3(0.0, 0.0, 0.0));
				RigidBodyComponent::SetAccel(Vec3(0.0, 0.0, 0.0));
			}
		}
	}

	RigidBodyComponent::HandleEvents(event);
	SpriteComponent::HandleEvents(event);
}



void Player::OnCollisionEnter(RigidBodyComponent& otherBody)
{
	if (Tile* t = dynamic_cast<Tile*>(&otherBody))
	{
		if (t->tileType == Tile::TileType::Normal)
		{
			std::cout << "Player has touched a Normal Tile." << std::endl;
		}
		if ((t->tileType == Tile::TileType::Refuel) && (t->pass == 0))
		{
			jetPower += 2.0f;
			std::cout << "Player has touched a Refuel Tile." << std::endl;
		}
		if (t->tileType == Tile::TileType::Hazard)
		{
			SetVelocity(Vec3(GetVelocity().x * -1 , 0.0, 0.0));
			std::cout << "Player has touched a Hazard Tile." << std::endl;
		}
	}
}
