
#include "Scene1.h"
#include "custom/Player.h"
#include "custom/MouseRay.h"
#include "core/Debug.h"
#include "core/TextureManager.h"


Scene1::Scene1()
{}

Scene1::~Scene1()
{
	
}

bool Scene1::OnCreate()
{
	
	std::cout << "=================\n" << "scene1 load start" << std::endl << "=================\n";
	objectList = std::make_unique<Manager>();

	//Setup the player
	player = new Player("Player", MATH::Vec3(0.0f, 20.0f, 70.0f));

	grass = new Grass("Grass", MATH::Vec3(0.0f, 1.0f, 0.0f), 700);
	box = new Box("Box", MATH::Vec3(0.0f, 0.0f, 0.0f));
	light = new LightObject("Light", MATH::Vec3(0.0f, 20.0f, 0.0f));
	mouseRay = std::make_unique<MouseRay>();
	objectList->AddGameObject(player);
	objectList->AddGameObject(grass);
	objectList->AddGameObject(box);
	objectList->AddGameObject(light);

	objectList->Init();

	PerformanceMonitor::InitMonitor();
	
	//SaveMapData();
	LoadMapData();
	return false;
}



void Scene1::OnDestroy()
{

	pElement = nullptr;
}

void Scene1::Update(const float deltaTime)
{
	Scene::Update(deltaTime);
	Camera::getInstance()->Update(deltaTime);
	objectList->CheckCollisions();
	objectList->Update(deltaTime);

}



void Scene1::Render() const
{
	
	Scene::Render();
	objectList->Render();
}

void Scene1::HandleEvents(const SDL_Event& event)
{
	Scene::HandleEvents(event);
	objectList->HandleEvents(event);
}

void Scene1::Reset()
{
	OnDestroy();
	OnCreate();
}

void Scene1::SaveMapData() const 
{
	Scene::SaveMapData();
}

void Scene1::LoadMapData()
{	
	Scene::LoadMapData();
}





