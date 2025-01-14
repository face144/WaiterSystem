#pragma once

#include "Core/Minimal.h"
#include "Requests/RequestHandler.h"


class FSimpleRequestHandler : public FRequestHandler
{
public:
	virtual bool CanHandle(const http::request<http::string_body>& Request) const override;
	virtual void Handle(const http::request<http::string_body>& Request) override;
};
