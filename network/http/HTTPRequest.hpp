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

	HTTPRequest(std::string request) { ProcessRequest(request); }

	HTTPRequest(const char* request) { ProcessRequest(request); }

	~HTTPRequest() {}

	bool ProcessRequest(string request)
	{
		smatch matches;

		if (regex_search(request, matches, requestLineRegex))
		{
			_method = matches[1];
			_path = matches[2];
			_protocol = matches[3];

			int submatches[] = { 1, 2 };
			regex_token_iterator<string::iterator> rend;
			regex_token_iterator<string::iterator> it(request.begin(), request.end(), pairsRegex, submatches);
			string key;
			string value;
			
			for (; it != rend; it++)
			{
				key = *it;
				value = *++it;
				_values[key] = value;
			}

			_isValid = true;
		}
		else
		{
			_isValid = false;
		}
		
		return _isValid;
	}

	bool ProcessRequest(const char* request) { return ProcessRequest(string(request)); }

	string BuildString() { return BuildString(_method, _path, _protocol); }

	string BuildString(const string method, const string path, const string protocol)
	{
		string toReturn;

		toReturn = method + " " + path + " " + protocol + "\n";

		for (map<string,string>::iterator it = _values.begin(); it != _values.end(); it++)
		{
			toReturn.append(it->first + ": " + it->second + "\n");
		}
		
		return toReturn;
	}

	string toString() { return BuildString(); }

	pair<map<string, string>::iterator, bool> AddValue(string valName)
	{
		return _values.insert(pair<string, string>(valName, string()));
	}

	pair<map<string, string>::iterator, bool> AddValue(string valName, string value)
	{
		return _values.insert(pair<string, string>(valName, value));
	}

	map<string, string>::iterator ValuesBegin() { return _values.begin(); }

	map<string, string>::iterator ValuesEnd() { return _values.end(); }

	int ValueCount() { return _values.size(); }

	bool IsValuesEmpty() { return _values.empty(); }

	void ClearValues() { _values.clear(); }

	// Getters
	bool IsValid() { return _isValid; }
	string Method() { return _method; }
	string Path() { return _path; }
	string Protocol() { return _protocol; }

	// Setters
	void SetHead(string method, string path, string protocol)
	{
		_method = method;
		_path = path;
		_protocol = protocol;
	}

	// Operators
	string& operator[](const string valName) { return _values[valName]; }
	
	string operator()() { return BuildString(); }

	string operator()(string method, string path, string protocol) { return BuildString(method, path, protocol);}

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