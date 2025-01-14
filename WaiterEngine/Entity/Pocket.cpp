#include "Pocket.h"

#include "Entity.h"


FPocket* FPocket::GlobalPocketPtr = nullptr;

FPocket::FPocket()
{
	GlobalPocketPtr = this;
}

FPocket::~FPocket() = default;

void FPocket::Initialize()
{
}

void FPocket::Tick(float DeltaTime)
{
	for (auto& Entity : EntityList)
	{		
		if (auto* const TickableEntity = dynamic_cast<ITickable*>(Entity.get());
			TickableEntity != nullptr)
		{
			TickableEntity->Tick(DeltaTime);
		}
	}
}

void FPocket::DestroyEntity(const std::shared_ptr<FEntity>& InEntity)
{
	for (uint64 i = 0; i < EntityList.size(); i++)
	{
		if (EntityList[i].get() == InEntity.get())
		{
			EntityList[i].reset();
			EntityList.erase(EntityList.begin() + static_cast<int64>(i));
		}
	}
}
