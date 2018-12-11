#include "RenderObject.h"

RenderObject::RenderObject(const physx::PxGeometry& geo, Vector3 pos, Vector4 color)
{
	physx::PxShape* shape = CreateShape(geo);
	renderItem = new RenderItem(shape, &physx::PxTransform(pos), color);
	shape->release();
}
