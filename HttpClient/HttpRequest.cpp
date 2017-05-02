#include"HttpRequest.h"
#include<cpprest\http_client.h>

using namespace web;
using namespace web::http;

using namespace Http;

static const std::map<HttpRequestMethod, web::http::method> RequestToStringMap = {
	{ HttpRequestMethod::Get, methods::GET },
	{ HttpRequestMethod::Post, methods::POST }
};

HttpRequest::HttpRequest(const HttpRequestMethod& method, const std::string& path)
	: request_(std::make_shared<web::http::http_request>(web::http::http_request(RequestToStringMap.at(method))))
{
}

void HttpRequest::SetRequestUri(const std::string& uri)
{
	request_->set_request_uri(utility::conversions::to_string_t(uri));
}

std::string HttpRequest::GetRequestUri() const
{
	return utility::conversions::to_utf8string(request_->request_uri().to_string());
}

void HttpRequest::AddRequestHeader(const std::string& name, const std::string& value)
{
	request_->headers().add(utility::conversions::to_string_t(name)
		, utility::conversions::to_string_t(value));
}

std::map<std::string, std::string> HttpRequest::GetHeaders()
{
	std::map<std::string, std::string> request_headers;

	for (auto it = request_->headers().begin(); it != request_->headers().end(); ++it)
	{
		request_headers.emplace(utility::conversions::to_utf8string(it->first),
			utility::conversions::to_utf8string(it->second));
	}

	return request_headers;
}

void HttpRequest::SetBody(const std::vector<unsigned char>& body)
{
	request_->set_body(body);
}

std::vector<unsigned char> HttpRequest::GetBody() const
{
	return request_->extract_vector().get();
}