#include <iostream>
#include <fstream>
#include <csignal>
#include <sys/stat.h>

#include "../network/tcp/TcpSocket.hpp"
#include "../network/tcp/TcpConnection.hpp"
#include "../network/http/HTTPRequest.hpp"
#include "../network/http/HTTPResponse.hpp"
#include "../network/http/HTTPResponseCodes.hpp"
#include "../general/globals.hpp"
#include "../general/Data.hpp"

#define BUFFER_SIZE 16384
#define ICO_SIZE 2333
//#define DEBUG

using namespace std;
using namespace LongREST::HTTP;
using namespace LongREST::TCP;
using namespace LongREST::General;

TcpSocket server;
struct sigaction sigIntHandler;

string GetHTML(string filename)
{
    ifstream inFile (filename.c_str());
    string out;
    string line;

    if (inFile.is_open())
    {
        while(getline(inFile, line))
        {
            out.append(line);
        }
    }

    inFile.close();
    return out;
}

int GetBinary(string filename, char* out, size_t size)
{
	ifstream inFile (filename.c_str(), ios::in | ios::binary);
	struct stat results;

	if (inFile.good())
	{
		if (stat(filename.c_str(), &results) == 0)
		{
			// The size of the file in bytes is in
			// results.st_size
			if (size < results.st_size) { return -2; }

			inFile.read(out, results.st_size);

			return results.st_size;
		}
		else { return -1; }
	}
	else { return -1; }
}

void CtrlCHandler(int sig)
{
	server.Close();
}

void SetupSigHandler()
{
	sigIntHandler.sa_handler = CtrlCHandler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;

   	sigaction(SIGINT, &sigIntHandler, NULL);
}

void PrintSection(string toPrint, string heading = "")
{
	cout << "-------" << heading << "------\n"
	<< toPrint <<
	"\n-------" << heading << "-------" << endl;
}
 
int main(int argc, char const *argv[])
{
    cout << "Program has began..." << endl;
	SetupSigHandler();

	TcpConnection con;
    char buffer[BUFFER_SIZE];
	char fileBuffer[BUFFER_SIZE];
	char ico[ICO_SIZE];
	string outHtml = GetHTML("resourses/index.html");
	int icoOutSize = GetBinary("resourses/favicon.ico", ico, ICO_SIZE);
	int readSize = 0;

	if (icoOutSize != ICO_SIZE) 
	{ 
		cout << "Ico size incorrect... Size: " << icoOutSize << endl; 
	}
    
    // Server setup
    server.Setup();
	server.Bind();
	server.Listen(3);

	// Continue to accept until the server isn't good anymore
	while ((con = server.Accept()).IsGood())
	{
		readSize = con.Read(buffer, BUFFER_SIZE);

		// Read and display the data
		if (readSize > 0)
		{
			buffer[readSize] = '\0';
			HTTPRequest request = HTTPRequest(string(buffer));
			HTTPResponse response;

			if (request.IsValid())
			{
				// Print buffer
				PrintSection(string(buffer));
				#ifdef DEBUG
				PrintSection(request.toString(), "ToString()");
				#endif

				response["Server"] = LONG_REST_VERSION;
				
				// Send html
				if (request.Path() == "/") 
				{ 
					response.SetStatusLine(request.Protocol(), OK, "OK");
					response["Content-Type"] = "text/html; charset=utf-8";
					string rq = response.BuildString(outHtml);
					
					// Send HTML page
					con.Send(rq.c_str(), rq.length(), 0);
					PrintSection(rq, "HTML-Response");
				}
				else if (request.Path() == "/favicon.ico") 
				{ 
					response.SetStatusLine(request.Protocol(), OK, "OK");
					response["Content-Type"] = "image/x-icon";
					size_t outSize = response.BuildBuffer(ico, icoOutSize, buffer, BUFFER_SIZE);

					con.Send(buffer, outSize); 

					buffer[outSize] = '\0';
					PrintSection(string(buffer), "Favicon-Response");
				}
				else 
				{
					// Assemble the filepath
					string filepath("resourses");
					filepath.append(request.Path());

					// Pull the file
					int count = GetBinary(filepath, fileBuffer, BUFFER_SIZE);

					// Send the file
					if (count > 0) 
					{
						response.SetStatusLine(request.Protocol(), OK, "OK");
						size_t resSize = response.BuildBuffer(fileBuffer, count, buffer, BUFFER_SIZE);

						// Since we convert to a string, add a null value
						buffer[resSize] = '\0';

						// DEBUG - Show buffer
						PrintSection(string(buffer), "Buffer InvCpy Test");

						if (resSize > 0) { con.Send(buffer, resSize); }
						else
						{
							// TODO: Add bad buffer error
						}
					}
					else
					{
						response.SetStatusLine(request.Protocol(), NOT_FOUND, "FILE NOT FOUND");
						string toSend = response.toString();
						con.Send(toSend.c_str(), toSend.size(), 0);
					}
				}
			}

			con.Close();
		}
		else
		{
			cout << "No values read from the buffer..." << endl;
		}
	}
	
	server.Close();
	cout << "Closed with style" << endl;

    return 0;
}
