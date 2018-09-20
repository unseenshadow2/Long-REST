#include <iostream>
#include <fstream>
#include <csignal>

#include "network/TcpSocket.hpp"
#include "network/TcpConnection.hpp"

#define BUFFER_SIZE 1024

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
    string outHtml = GetHTML("resourses/index.html");
    
    server.Setup();
	server.Listen(3);

	while ((con = server.Accept()).IsGood())
	{
		// Read and display the data
		if (con.Read(buffer, BUFFER_SIZE) > 0)
		{
			// Print buffer
			cout << "------------------------\n" <<
			buffer <<
			"\n------------------------" << endl;

			// Send html
			con.Send(outHtml.c_str(), outHtml.length(), 0);

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
