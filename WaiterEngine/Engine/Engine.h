#pragma once


#include "Core/Minimal.h"
#include "MainLoop.h"
#include "Entity/Pocket.h"
#include <thread>


enum class EThreads
{
	Unknown,
	Engine,
	MainLoop,
};

class FEngine
{
public:
	static FEngine& Get();
	
	WAITERENGINE_API FEngine();
	WAITERENGINE_API virtual ~FEngine();

	template<typename TPocketClass>
	void Initialize()
	{
		static_assert(std::is_base_of_v<FPocket, TPocketClass>);
		EntityPocket = std::make_shared<TPocketClass>();
		SetupEngineLoop();
	}
	
	FPocket& GetPocket();
	
	static EThreads& GetCurrentThread();
protected:
	
private:
	static FEngine* GlobalEnginePtr;
	
	FMainLoop EngineLoop;
	std::shared_ptr<FPocket> EntityPocket;

	std::atomic<bool> bShouldStopLoop;
	std::thread EngineLoopThread;

	static void SignalShutdown(int Sig);

	WAITERENGINE_API void SetupEngineLoop();
	void TickEntities();
	
	inline static thread_local EThreads CurrentThread = EThreads::Unknown;
};
