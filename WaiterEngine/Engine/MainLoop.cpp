#include "MainLoop.h"

#include <chrono>
#include <stdexcept>
#include <thread>
#include "OutputDevice/DefaultOutputDevice.h"


FMainLoop::FMainLoop(float InTickRate)
{
	DeltaTime = 0;
	TickRate = InTickRate;
	ShouldStopLoop = [this] { return true; };
	bIsRunning = false;
}

void FMainLoop::StartLoop()
{
	if (!ShouldStopLoop)
	{
		throw std::runtime_error("Stop loop condition should be set."
						   " Set it with SetLoopStopCondition function.");
	}

	bIsRunning = true;
	do
	{
		const auto PreLoop = std::chrono::high_resolution_clock::now();
		
		for (std::function<void()>& Callable : CallableList)
		{
			Callable();
		}
		
		const auto PostLoop = std::chrono::high_resolution_clock::now();
		const auto SecondsPerCall = std::chrono::duration<float>(1.f / TickRate);

		std::chrono::duration<float> LoopTime = PostLoop - PreLoop;
		
		if (LoopTime < SecondsPerCall)
		{
			auto TimeToSleep = SecondsPerCall - std::chrono::duration_cast<std::chrono::seconds>(LoopTime);
			std::this_thread::sleep_for(TimeToSleep);
			DeltaTime = LoopTime.count() + TimeToSleep.count();
		}
		else
		{
			DeltaTime = LoopTime.count();
		}
		
		/*
		std::ostringstream TickTimeLog;
		TickTimeLog << "Tick Time: " << DeltaTime;
		FDefaultOutputDevice::Get().Write(TickTimeLog.str(), EVerbosity::Log);
		*/
	}
	while (!ShouldStopLoop());
}

void FMainLoop::SetLoopStopCondition(const std::function<bool()>& InShouldStopLoop)
{
	std::scoped_lock Lock(EngineLoopMutex);
	ShouldStopLoop = InShouldStopLoop;
}

void FMainLoop::AddLoopStep(const std::function<void()>& func)
{
	std::scoped_lock Lock(EngineLoopMutex);
	CallableList.push_back(func);
}

bool FMainLoop::IsRunning() const
{
	return bIsRunning;
}

float FMainLoop::GetDeltaTime() const
{
	return DeltaTime;
}
