#ifndef SCENE_H
#define SCENE_H

#include "sdl/SDL.h"
#include "components/ECS.h"
#include "External/tinyxml2.h"
#include "imgui/imgui.h"
#include "graphics/CustomUI.h"
#include "custom/Primitives/Primitives.h"
#include "VMath.h"
#include "custom/MouseRay.h"
#include <memory>
#include <stdio.h>
using namespace tinyxml2;


//! Scene Class
/*! Standard scene class that all scenes will inherit from  */
class Scene
{
protected:
	//! MouseRay
	/*! Converts mouse position on screen to world space to allow us to do mouse picking  */
	MouseRay mouseRay;

	//! PerformancePanel Class Instance
	/*! Renders and Updates all the Performance variables we are wanting using IMGUI  */
	CustomUI::PerformancePanel performancePanel;
	char* name_;

	//! GameObject Pointer
	/*! Stores our object that was most recently selected using our mouseRay */
	std::vector<CustomUI::PropertiesPanel*> propertiesPanels;

	//! Const Char* Pointer
	/*! Stores our objects that can be spawned */
	const char* objClasses[3] = { "Plane", "Box","Sphere" };

	//! Create object with object ID function
	/*! Used when we want to runtime spawn objects depending on the given ID */
	void CreateObjWithID(const MATH::Vec3& pos_, const MATH::Vec3& rot_, const MATH::Vec3& scale_, const char* objName, const char* IDName) const;

	//! Check Existing Panel Function
	/*! Check's if the clicked object is already in the vector of propertiesPannels: 
	if so then it deletes that objects properties panel in the vector*/
	void CheckExistingPanel(GameObject* obj);

	//! Check Intersection function
	/*! Checks if the MouseRay has intersected with a object */
	bool CheckIntersection(const MouseRay& ray, const MATH::Vec3& origin, GameObject* obj) const;

public:
	std::unique_ptr<Manager> objectList;

	// Used for saving and loading xml document information
	XMLElement* pElement;
	Scene();
	virtual ~Scene();

	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Update(const float deltaTime);
	virtual void Render() const;
	virtual void HandleEvents(const SDL_Event& event);
	virtual void Reset() = 0;
	virtual void SaveMapData() const;
	virtual void LoadMapData();


	std::string CheckAtributeValue(int i) const;
	
};

#endif