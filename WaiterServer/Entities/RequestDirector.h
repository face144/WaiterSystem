#pragma once

#include "Core/Minimal.h"
#include "Entity/Entity.h"
#include <map>

namespace std
{
	class thread;
}

class FRequestDirector : public FEntity
{	
public:
	FRequestDirector() = default;
	virtual ~FRequestDirector() override;

	virtual void Start() override;
	virtual void Tick(const float DeltaTime) override;

private:
	std::map<std::shared_ptr<std::thread>, std::shared_ptr<std::atomic<bool>>> HandlerThreads;
};


