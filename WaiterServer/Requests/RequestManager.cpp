#include "RequestManager.h"

#include "RequestHandler.h"
#include "Handlers/SimpleRequestHandler.h"


FRequestManager::FRequestManager()
{
	// Handlers.push_back(std::make_shared<FRequestHandler>());
	Handlers.push_back(std::make_shared<FSimpleRequestHandler>());
}

void FRequestManager::HandleRequest(const std::shared_ptr<std::atomic<bool>>& ActiveFlag,
									const http::request<http::string_body>& Request)
{
	for (auto& Handler : Handlers)
	{
		if (!ActiveFlag->load())
		{
			return;
		}
		
		if (Handler->CanHandle(Request))
		{
			Handler->Handle(Request);
		}
	}
	
	ActiveFlag->store(false);
}
