#include "Box.h"

Box::Box()
{
}

Box::Box(const char* name, MATH::Vec3 position) : MeshRenderer("Cube.fbx")
{
	this->name = name;
	transform.setPos(position);

	transform.scale = Vec3(1.0f, 1.0f, 1.0f);

	typeID = Type::Box;

	RigidBody3D::Init(this);
	MeshRenderer::Init(this);

	MeshRenderer::CreateShader("src/graphics/shaders/DefaultVert.glsl", "src/graphics/shaders/DefaultFrag.glsl");

}

Box::~Box()
{
}

void Box::Update(const float deltaTime)
{
	//transform.rotation.y += deltaTime;
	transform.Update(deltaTime);
	MeshRenderer::Update(deltaTime);
	RigidBody3D::Update(deltaTime);
}

void Box::Render() const
{
	MeshRenderer::Render();
}

void Box::HandleEvents(const SDL_Event& event)
{
	MeshRenderer::HandleEvents(event);
}

void Box::OnCollisionEnter(RigidBody3D& otherBody)
{
	//std::cout << this->name << " Collided With: " << otherBody.gameobject->name << std::endl;
}
