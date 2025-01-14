#include "Entity.h"

#include <cassert>

#include "Engine/Engine.h"


void FEntity::SetPocketContext(FPocket* Pocket)
{
	PocketContext = Pocket;
}

FPocket* FEntity::GetPocket() const
{
	return PocketContext;
}

void FEntity::Start()
{
	assert(FEngine::Get().GetCurrentThread() == EThreads::MainLoop);
}

void FEntity::Tick(float DeltaTime)
{
	assert(FEngine::Get().GetCurrentThread() == EThreads::MainLoop);
}
