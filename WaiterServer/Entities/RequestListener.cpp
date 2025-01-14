#include "RequestListener.h"

#include "boost/asio.hpp"
#include "Engine/Engine.h"
#include "OutputDevice/DefaultOutputDevice.h"


FRequestListener::FRequestListener() : bShutdownListener(false), ListeningPort(8080)
{
	
}

FRequestListener::~FRequestListener()
{
	bShutdownListener.store(true);
	ListenerThread.join();
}

void FRequestListener::Start()
{
	FEntity::Start();

	StartListenerThread();
}

void FRequestListener::Tick(float DeltaTime)
{
	FEntity::Tick(DeltaTime);
	
	if (Listener != nullptr && !Listener->RequestQueue.empty())
	{
		http::request<http::string_body> Request = Listener->RequestQueue.front();
		Listener->RequestQueue.pop();
		OnRequestReceived.Publish(Request);
	}
}

void FRequestListener::StartListenerThread()
{
	std::ostringstream ListenerStartLog;
	ListenerStartLog << "Starting listener thread on port " << ListeningPort;
	FDefaultOutputDevice::Get().Write(this, ListenerStartLog.str(), EVerbosity::Log);
	
	ListenerThread = std::thread([this]
	{
		Listener = std::make_unique<FListener>();		
		Listener->Start(ListeningPort, bShutdownListener);
	});
}

void FRequestListener::FListener::Start(int16 InListeningPort, const std::atomic<bool>& bShutdownSignal)
{
	try
	{
		net::io_context ioc;
		tcp::acceptor acceptor(ioc, tcp::endpoint(tcp::v4(), InListeningPort));
		acceptor.non_blocking(true);	
		
		do
		{
			tcp::socket socket(ioc);
			boost::system::error_code ec = acceptor.accept(socket, ec);
			if (!ec)
			{
				beast::flat_buffer buffer;
				http::request<http::string_body> req;
				http::read(socket, buffer, req);

				FDefaultOutputDevice::Get().Write(this, (std::ostringstream() << "Received new request").str(), EVerbosity::Log);
				RequestQueue.emplace(req);
			}
		}
		while (!bShutdownSignal.load());
	}
	catch (const std::exception& e)
	{
		FDefaultOutputDevice::Get().Write(this, (std::ostringstream() << "Error: " << e.what()).str(), EVerbosity::Log);
	}
}
