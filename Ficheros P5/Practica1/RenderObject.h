#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"

class RenderObject // objeto pintable, sin logica
{
private:
	RenderItem* renderItem = nullptr;
public:
	RenderObject() {}
	RenderObject(const physx::PxGeometry& geo, Vector3 pos, Vector4 color);
	virtual ~RenderObject() {}
};

