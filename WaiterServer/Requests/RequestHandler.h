#pragma once

#include "Core/Minimal.h"
#include "boost/beast.hpp"


namespace net = boost::asio;         
namespace beast = boost::beast;     
namespace http = beast::http;       
using tcp = net::ip::tcp;

class FRequestHandler
{
public:
	FRequestHandler() = default;
	virtual ~FRequestHandler() = default;
	
	[[nodiscard]] virtual bool CanHandle(const http::request<http::string_body>& Request) const = 0;
	virtual void Handle(const http::request<http::string_body>& Request) = 0;

protected:

private:
};
