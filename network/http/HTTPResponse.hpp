#ifndef HTTP_RESPONSE_HEADER
#define HTTP_RESPONSE_HEADER

#include <map>
#include <regex>
#include <string>

#include "HTTPResponseCodes.hpp"
#include "HTTPHeader.hpp"

using namespace std;

class HTTPResponse : public HTTPHeader
{
public:
	HTTPResponse() {}
	HTTPResponse(string response) { ProcessResponse(response); }
	HTTPResponse(const char* response) { ProcessResponse(response); }

	bool ProcessResponse(string response)
	{
		smatch matches;

		if (regex_search(response, matches, requestLineRegex))
		{
			_protocol = matches[1];
			_statusCode = matches[2];
			_reasonPhrase = matches[3];

			// Fill our pairs
			HTTPHeader::Process(response);
		}
		else
		{
			_isValid = false;
		}
		
		return _isValid;
	}

	string BuildString() { BuildString(_protocol, _statusCode, _reasonPhrase); }
	string BuildString(string protocol, string statusCode, string reasonPhrase) 
	{
		string toReturn;

		toReturn = protocol + " " + statusCode + " " + reasonPhrase + HTTP_NEW_LINE;

		// Fill our pairs
		HTTPHeader::BuildString(toReturn);
		
		return toReturn;
	}

	void SetStatusLine(string protocol, string statusCode, string reasonPhrase)
	{
		_protocol = protocol;
		_statusCode = statusCode;
		_reasonPhrase = reasonPhrase;
	}

	// Getters
	string Protocol() { return _protocol; }
	string StatusCode() { return _statusCode; }
	string ReasonPhrase() { return _reasonPhrase; }
	
private:
	string _protocol;
	string _statusCode;
	string _reasonPhrase;

	// Regexes
	const regex requestLineRegex = regex(R"((\w+)\s+(\S+)\s+(.+)(?:\n|\r|\n\r|\r\n))");
};

#endif