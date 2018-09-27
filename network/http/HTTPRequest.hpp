#ifndef HTTP_REQUEST_HEADER
#define HTTP_REQUEST_HEADER

#include <string>
#include <regex>
#include <map>

#include "HTTPHeader.hpp"

using namespace std;

class HTTPRequest : public HTTPHeader
{
public:
	HTTPRequest() {}
	HTTPRequest(std::string request) { Process(request); }
	HTTPRequest(const char* request) { Process(request); }

	~HTTPRequest() {}

	//bool Process(const char* request) { return Process(string(request)); }
	bool Process(string request)
	{
		smatch matches;

		if (regex_search(request, matches, requestLineRegex))
		{
			_method = matches[1];
			_path = matches[2];
			_protocol = matches[3];

			// Fill our pairs
			HTTPHeader::Process(request);
		}
		else
		{
			_isValid = false;
		}
		
		return _isValid;
	}

	string BuildString() { return BuildString(_method, _path, _protocol); }
	string BuildString(const string method, const string path, const string protocol)
	{
		string toReturn = method + " " + path + " " + protocol + HTTP_NEW_LINE;

		// Fill our pairs
		HTTPHeader::BuildString(toReturn);
		toReturn += HTTP_NEW_LINE;
		
		return toReturn;
	}

	// Getters
	string Method() { return _method; }
	string Path() { return _path; }
	string Protocol() { return _protocol; }

	// Setters
	void SetRequestLine(string method, string path, string protocol)
	{
		_method = method;
		_path = path;
		_protocol = protocol;
	}

	// Operators
	string operator()(string method, string path, string protocol) { return BuildString(method, path, protocol);}

private:
	string _method;
	string _path;
	string _protocol;

	// Regexes
	const regex requestLineRegex = regex(R"((\w+)\s+(\S+)\s+(\S+)(?:\n|\r|\n\r|\r\n))");
};

#endif