#include <iostream>
#include <fstream>
#include <csignal>
#include <sys/stat.h>

#include "network/tcp/TcpSocket.hpp"
#include "network/tcp/TcpConnection.hpp"
#include "network/http/HTTPRequest.hpp"

#define BUFFER_SIZE 16384
#define ICO_SIZE 2333

using namespace std;

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
 
int main(int argc, char const *argv[])
{
    cout << "Program has began..." << endl;
	SetupSigHandler();

	TcpConnection con;
    char buffer[BUFFER_SIZE];
	char ico[ICO_SIZE];
	string outHtml = GetHTML("resourses/index.html");
	int icoOutSize = GetBinary("resourses/favicon.ico", ico, ICO_SIZE);

	if (icoOutSize != ICO_SIZE) 
	{ 
		cout << "Ico size incorrect... Size: " << icoOutSize << endl; 
	}
    
    // Server setup
    server.Setup();
	server.Listen(3);

	// Continue to accept until the server isn't good anymore
	while ((con = server.Accept()).IsGood())
	{
		// Read and display the data
		if (con.Read(buffer, BUFFER_SIZE) > 0)
		{
			HTTPRequest request = HTTPRequest((string(buffer)));

			// Print buffer
			cout << "------------------------\n" <<
			buffer <<
			"\n------------------------" << endl;
			
			// Send html
			if (request.Path() == "/") { con.Send(outHtml.c_str(), outHtml.length(), 0); }
			else if (request.Path() == "/favicon.ico") { con.Send(ico, icoOutSize); }
			else 
			{
				// Assemble the filepath
				string filepath("resourses");
				filepath.append(request.Path());

				// Pull the file
				int count = GetBinary(filepath, buffer, BUFFER_SIZE);

				// Send the file
				if (count > 0) { con.Send(buffer, count); }
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
