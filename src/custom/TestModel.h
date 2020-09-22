#pragma once
#include "components/ECS.h"
#include "components/Components.h"

class TestModel : public GameObject, public MeshRenderer, public RigidBody3D
{
public:
	TestModel();
	TestModel(const char* name, MATH::Vec3 position);
	~TestModel();


	// Inherited via GameObject
	void Update(const float deltaTime) override;
	void Render() const override;
	void HandleEvents(const SDL_Event& event) override;

	virtual void OnCollisionEnter(RigidBody3D& otherBody) override;


private:



};

