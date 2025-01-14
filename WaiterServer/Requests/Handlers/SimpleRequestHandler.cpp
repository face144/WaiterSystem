#include "SimpleRequestHandler.h"

#include <boost/json/parse.hpp>
#include "OutputDevice/DefaultOutputDevice.h"


bool FSimpleRequestHandler::CanHandle(const http::request<http::string_body>& Request) const
{
	try
	{
		boost::json::parse(Request.body());
	}
	catch (...)
	{
		return false;
	}

	return true;
}

void FSimpleRequestHandler::Handle(const http::request<http::string_body>& Request)
{
	std::string JsonString = Request.body();
	boost::json::value JsonValue;
	try
	{
		 JsonValue = boost::json::parse(JsonString);
	}
	catch (...)
	{
		FDefaultOutputDevice::Get().Write(this, "Error parsing json", EVerbosity::Error);
		return;
	}

	boost::json::object JsonObject = JsonValue.as_object();
	if (!JsonObject.contains("room") || !JsonObject.contains("prepaid") || !JsonObject.contains("order"))
	{
		FDefaultOutputDevice::Get().Write(this, "Received incomplete request", EVerbosity::Warning);
		return;
	}


	auto& RoomId = JsonObject["room"].as_string();
	auto& bIsPrePaid = JsonObject["prepaid"].as_bool();
	auto& OrderList = JsonObject["order"].as_array();

	std::ostringstream OrderString;
	OrderString << "\n";
	OrderString << "\n";
	OrderString << "NEW ORDER RECEIVED!" << "\n";
	OrderString << "Room ID: " << RoomId << "\n";
	OrderString << "Pre-Paid order: " << (bIsPrePaid ? "yes" : "no") << "\n";
	OrderString << "Order: ";
	for (auto& Item : OrderList)
	{
		OrderString << Item.as_string();
		OrderString << (Item == OrderList.back() ? ".\n" : ", ");
	}
	
	FDefaultOutputDevice::Get().Write(this, OrderString.str(), EVerbosity::Log);
}
