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

	string BuildString() { return BuildString(_protocol, _statusCode, _reasonPhrase); }
	string BuildString(string page) { return BuildString(_protocol, _statusCode, _reasonPhrase, page); }
	string BuildString(string protocol, string statusCode, string reasonPhrase) 
	{
		string toReturn;

		toReturn = protocol + " " + statusCode + " " + reasonPhrase + HTTP_NEW_LINE;

		// Fill our pairs
		HTTPHeader::BuildString(toReturn);
		
		return toReturn;
	}

	// This version is intended to include an html, js, css, or other plaintext pages
	string BuildString(string protocol, string statusCode, string reasonPhrase, string page)
	{
		string toReturn;

		toReturn = BuildString(protocol, statusCode, reasonPhrase);
		toReturn.append(HTTP_NEW_LINE);
		toReturn.append(page);
		
		return toReturn;
	}

	// The BuildBuffer() functions are intended for use with binary file types, such as .png
	size_t BuildBuffer(void* const data, size_t dataSize, void* out, size_t outSize)
	{
		string header = BuildString();
		size_t totalSize = dataSize + header.size() + HTTP_NEW_LINE_SIZE;
		char * pos;

		// Exception check
		if (totalSize > outSize) 
		{
			// TODO: Add exception
			return 0;
		}
		else
		{
			pos = (char*)memcpy(out, header.c_str(), header.size());
			pos += header.size();
			memcpy(pos, HTTP_NEW_LINE, HTTP_NEW_LINE_SIZE);
			pos += HTTP_NEW_LINE_SIZE;
			memcpy(pos, data, dataSize);
			
			return totalSize;
		}
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