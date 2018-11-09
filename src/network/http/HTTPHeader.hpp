#ifndef HTTP_HEADER_HEADER
#define HTTP_HEADER_HEADER

#define HTTP_NEW_LINE "\r\n"
#define HTTP_NEW_LINE_SIZE 2

#include <string>
#include <regex>
#include <map>

using namespace std;

namespace LongREST
{
	namespace HTTP
	{
		class HTTPHeader
		{
		public:
			virtual bool Process(const char* toProcess) { return Process(toProcess); }
			virtual bool Process(string toProcess)
			{
				int submatches[] = { 1, 2 };
				regex_token_iterator<string::iterator> rend;
				regex_token_iterator<string::iterator> it(toProcess.begin(), toProcess.end(), pairsRegex, submatches);
				string key;
				string value;
				
				for (; it != rend; it++)
				{
					key = *it;
					value = *++it;
					_values[key] = value;
				}

				_isValid = true;
				return _isValid;
			}

			virtual string BuildString() { string toReturn; return BuildString(toReturn); }
			virtual string toString() { return BuildString(); }

			pair<map<string, string>::iterator, bool> AddValue(string valName) { return _values.insert(pair<string, string>(valName, string())); }
			pair<map<string, string>::iterator, bool> AddValue(string valName, string value) { return _values.insert(pair<string, string>(valName, value)); }

			map<string, string>::iterator ValuesBegin() { return _values.begin(); }
			map<string, string>::iterator ValuesEnd() { return _values.end(); }

			int ValueCount() { return _values.size(); }
			bool IsValuesEmpty() { return _values.empty(); }
			void ClearValues() { _values.clear(); }

			// Getters
			bool IsValid() { return _isValid; }

			// Operators
			virtual string& operator[](const string valName) { return _values[valName]; }
			virtual string operator()() { return BuildString(); }
			
		protected:
			virtual string BuildString(string& outString)
			{
				for (map<string,string>::iterator it = _values.begin(); it != _values.end(); it++)
				{
					outString.append(it->first + ": " + it->second + HTTP_NEW_LINE);
				}

				return outString;
			}

			map<string, string> _values;

			bool _isValid = false;

			const regex pairsRegex = regex(R"((\S+): (.+))");

		private:
		};
	}
}

#endif