#ifndef PLAYER_H
#define PLAYER_H

#include "components/Components.h"

//Player inherits from gameobject, components are also added via inheritance
class Player : public GameObject, public SpriteComponent, public RigidBodyComponent
{
private:



public:

	Player();
	//Whenever making a gameobject make sure to give it a name and a position
	Player(const char* n, const MATH::Vec3& trans);
	virtual ~Player();



	//Inherited via GameObject
	virtual void Update(const float deltaTime) override;
	virtual void Render() const override;
	virtual void HandleEvents(const SDL_Event& event) override;


	//Inherited via RigidBodyComponent
	virtual void OnCollisionEnter() override;

};

#endif