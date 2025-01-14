#pragma once

#include "Core/Minimal.h"
#include <functional>
#include <mutex>
#include <vector>


class FMainLoop
{
public:
	explicit FMainLoop(float InTickRate = 8.f);

	void StartLoop();
	void SetLoopStopCondition(const std::function<bool()>& InShouldStopLoop);
	void AddLoopStep(const std::function<void()>& func);
	[[nodiscard]] bool IsRunning() const;
	[[nodiscard]] float GetDeltaTime() const;

protected:
	float TickRate;
	
private:
	std::vector<std::function<void()>> CallableList;
	std::function<bool()> ShouldStopLoop;
	bool bIsRunning;
	float DeltaTime;

	std::mutex EngineLoopMutex;
};
