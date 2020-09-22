#ifndef COLLIDER3D_H
#define COLLIDER3D_H

//Not quite a component but similar enough to be in the same folder path
//Colliders define how rigidbodies will collide
struct Collider3D
{
public:

	enum class shape
	{
		Sphere,
		AABB
	};

	shape colliderShape;
	bool isMoveable, isTrigger,isWiremesh;
	MATH::Vec3 size;

	Collider3D()
	{
		colliderShape = shape::AABB;
		isMoveable = true;
		isTrigger = false;
		//isWiremesh = false;
		size = MATH::Vec3(1.0f, 1.0f, 1.0f);
	}
};
#endif