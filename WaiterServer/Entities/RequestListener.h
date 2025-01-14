#pragma once

#include "Core/Minimal.h"
#include "Entity/Entity.h"
#include <thread>
#include <atomic>
#include <queue>

#include "boost/beast.hpp"
#include "Event/Event.h"


namespace net = boost::asio;         
namespace beast = boost::beast;     
namespace http = beast::http;       
using tcp = net::ip::tcp;

class FRequestListener : public FEntity
{
	class FListener
	{
	public:	
		FListener() = default;
		void Start(int16 InListeningPort, const std::atomic<bool>& bShutdownSignal);

		std::queue<http::request<http::string_body>> RequestQueue;
	};
	
public:
	FEvent<http::request<http::string_body>> OnRequestReceived;
	FRequestListener();
	virtual ~FRequestListener() override;
	
	virtual void Start() override;
	virtual void Tick(float DeltaTime) override;

private:	
	void StartListenerThread();
	
	std::thread ListenerThread;
	std::atomic<bool> bShutdownListener;
	int16 ListeningPort;
	
	std::unique_ptr<FListener> Listener;
};
