#pragma once

#include <memory>

#include "Core/Minimal.h"
#include "TickSystem/Tickable.h"

class FPocket;

class WAITERENGINE_API FEntity : public ITickable
{
public:
	FEntity() = default;

	void SetPocketContext(FPocket* Pocket);

	[[nodiscard]] FPocket* GetPocket() const;
	
	virtual void Start();
	virtual void Tick(float DeltaTime) override;
	
private:
	FPocket* PocketContext;
};
