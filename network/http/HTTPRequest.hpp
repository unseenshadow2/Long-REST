#ifndef HTTP_REQUEST_HEADER
#define HTTP_REQUEST_HEADER

#include <string>
#include <regex>
#include <map>

using namespace std;

class HTTPRequest
{
public:
	HTTPRequest() {}

	HTTPRequest(std::string request)
	{
		ProcessRequest(request);
	}

	HTTPRequest(const char* request)
	{
		ProcessRequest(request);
	}

	~HTTPRequest() {}

	bool ProcessRequest(std::string request)
	{
		smatch matches;

		if (regex_search(request, matches, requestLineRegex))
		{
			_method = matches[1];
			_path = matches[2];
			_protocol = matches[3];
		}

		// TODO: Pull out Key:Value pairs

		return _isValid;
	}

	bool ProcessRequest(const char* request)
	{
		// TODO: Build out this function
		return _isValid;
	}

	string BuildString() { return BuildString(_method, _path, _protocol); }

	string BuildString(string method, string path, string protocol)
	{
		string toReturn;
		// TODO: Build out this function
		return toReturn;
	}

	// Getters
	bool IsValid() { return _isValid; }
	string Method() { return _method; }
	string Path() { return _path; }
	string Protocol() { return _protocol; }

	// TODO: Overload[] and ()

private:
	string _method;
	string _path;
	string _protocol;

	map<string, string> _values;

	bool _isValid = false;

	// Regexes
	const regex requestLineRegex = regex(R"((\w+)\s+(\S+)\s+(\S+)(?:\n|\r|\n\r|\r\n))");
	const regex pairsRegex = regex(R"((\S+): (.+))");
};

#endif