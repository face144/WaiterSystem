#include "Engine.h"

#include <stdexcept>
#include <csignal>
#include <iostream>
#include <thread>

#include "OutputDevice/DefaultOutputDevice.h"


FEngine* FEngine::GlobalEnginePtr = nullptr;

FEngine& FEngine::Get()
{
	if (GlobalEnginePtr == nullptr)
	{
		throw std::runtime_error("FEngine::Get(): GlobalEnginePtr is null");
	}
	
	return *GlobalEnginePtr;
}

FEngine::FEngine() : EngineLoop(FMainLoop(1.f))
{
	GlobalEnginePtr = this;
	bShouldStopLoop = false;
	CurrentThread = EThreads::Engine;
}

FEngine::~FEngine()
{
	FDefaultOutputDevice::Get().Write("Shutting down engine.", EVerbosity::Log);
}

FPocket& FEngine::GetPocket()
{
	return *EntityPocket;
}

EThreads& FEngine::GetCurrentThread()
{
	return CurrentThread;
}

void FEngine::SignalShutdown(int Sig)
{
	GlobalEnginePtr->bShouldStopLoop = true;
}

void FEngine::SetupEngineLoop()
{
	_crt_signal_t RetCode = std::signal(SIGINT, &FEngine::SignalShutdown);
	
	EngineLoop.SetLoopStopCondition([this] { return bShouldStopLoop.load(); });
	EngineLoop.AddLoopStep([this]{ TickEntities(); });

	EngineLoopThread = std::thread([this]
	{
		CurrentThread = EThreads::MainLoop;		
		EntityPocket->Initialize();
		
		EngineLoop.StartLoop();
	});
	
	EngineLoopThread.join();

	FDefaultOutputDevice::Get().Write("Requested exit from engine", EVerbosity::Log);
}

void FEngine::TickEntities()
{
	EntityPocket->Tick(EngineLoop.GetDeltaTime());
}
