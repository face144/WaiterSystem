#pragma once

#include "Core/Minimal.h"
#include <memory>
#include <vector>
#include "boost/beast.hpp"


namespace net = boost::asio;         
namespace beast = boost::beast;     
namespace http = beast::http;       
using tcp = net::ip::tcp;
class FRequestHandler;

class FRequestManager
{
public:
	FRequestManager();
	
	void HandleRequest(const std::shared_ptr<std::atomic<bool>>& ActiveFlag, const http::request<http::string_body>& Request);

private:
	std::vector<std::shared_ptr<FRequestHandler>> Handlers;
};
