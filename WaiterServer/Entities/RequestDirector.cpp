#include "RequestDirector.h"

#include "RequestListener.h"
#include "Entity/Pocket.h"
#include "OutputDevice/DefaultOutputDevice.h"
#include <boost/json.hpp>
#include <boost/beast/http.hpp>
#include "Requests/RequestManager.h"


namespace http = boost::beast::http;

FRequestDirector::~FRequestDirector()
{
	for (auto& ThreadFlags : HandlerThreads)
	{
		auto& Thread = ThreadFlags.first;
		auto& bIsActive = ThreadFlags.second;
		
		if (Thread->joinable())
		{			
			bIsActive->store(false);
			Thread->join();
		}
	}
}

void FRequestDirector::Start()
{
	FEntity::Start();
	std::shared_ptr<FRequestListener> RequestListener = GetPocket()->FindEntity<FRequestListener>();
	RequestListener->OnRequestReceived.Subscribe([this] (const http::request<http::string_body>& Request)
	{
		FDefaultOutputDevice::Get().Write(this, "Processing new request", EVerbosity::Log);

		std::shared_ptr<std::atomic<bool>> FinishFlag = std::make_shared<std::atomic<bool>>(true);
		auto RequestHandler = std::make_shared<std::thread>([this, Request, FinishFlag]
		{
			FRequestManager Manager;
			Manager.HandleRequest(FinishFlag, Request);
		});
		
		HandlerThreads.emplace(RequestHandler, FinishFlag);
	});
}

void FRequestDirector::Tick(const float DeltaTime)
{
	FEntity::Tick(DeltaTime);

	if (!HandlerThreads.empty())
	{
		for (auto It = HandlerThreads.begin(); It != HandlerThreads.end(); )
		{
			auto& Thread = It->first;
			const auto& bIsActive = It->second;

			if (!bIsActive->load() && Thread->joinable())
			{
				Thread->join();
				It = HandlerThreads.erase(It);
			}
			else
			{
				++It;
			}
		}
		
		std::ostringstream ActiveWorkersMessages;
		ActiveWorkersMessages << "Active request workers: " << HandlerThreads.size();
		FDefaultOutputDevice::Get().Write(this, ActiveWorkersMessages.str(), EVerbosity::Log);
	}
}
