#pragma once

#include "Core/Minimal.h"


class WAITERENGINE_API ITickable
{
public:
	ITickable() = default;
	virtual ~ITickable() = default;
	
	virtual void Tick(float DeltaTime) = 0;
	
};
