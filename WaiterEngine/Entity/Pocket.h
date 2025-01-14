#pragma once

#include "Core/Minimal.h"
#include <memory>
#include <vector>
#include "Entity.h"
#include "TickSystem/Tickable.h"


class FEntity;

class FPocket : public ITickable
{
public:	
	WAITERENGINE_API FPocket();
	WAITERENGINE_API ~FPocket() override;

	WAITERENGINE_API virtual void Initialize();
	WAITERENGINE_API virtual void Tick(float DeltaTime) override;

	template<typename T>
	std::shared_ptr<T> InstantiateEntity();

	WAITERENGINE_API void DestroyEntity(const std::shared_ptr<FEntity>& InEntity);

	template<typename T>
	std::shared_ptr<T> FindEntity();

private:
	WAITERENGINE_API static FPocket* GlobalPocketPtr;
	
	std::vector<std::shared_ptr<FEntity>> EntityList;
};

template <typename T>
std::shared_ptr<T> FPocket::InstantiateEntity()
{	
	static_assert(std::is_base_of_v<FEntity, T>);
	// assert(FEngine::Get().GetCurrentThread() == EThreads::MainLoop);

	std::shared_ptr<T> NewEntity = std::make_shared<T>();
	std::shared_ptr<FEntity> NewEntityBase = NewEntity;

	GlobalPocketPtr->EntityList.push_back(NewEntityBase);
	NewEntityBase->SetPocketContext(GlobalPocketPtr);
	NewEntityBase->Start();
		
	return NewEntity;
}

template <typename T>
std::shared_ptr<T> FPocket::FindEntity()
{
	static_assert(std::is_base_of_v<FEntity, T>);
    
	for (auto& Entity : EntityList)
	{
		if (auto CastedEntity = std::dynamic_pointer_cast<T>(Entity); CastedEntity != nullptr)
		{
			return CastedEntity;
		}
	}
    
	return nullptr;
}
