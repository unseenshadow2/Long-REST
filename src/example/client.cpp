#include <iostream>
#include <chrono>
#include <thread>

#include "../network/tcp/TcpSocket.hpp"
#include "../network/tcp/TcpConnection.hpp"
#include "../network/http/HTTPRequest.hpp"
#include "../network/http/HTTPResponse.hpp"
#include "../network/http/HTTPResponseCodes.hpp"
#include "../general/globals.hpp"
#include "../general/Data.hpp"

using namespace std;
using namespace LongREST::HTTP;
using namespace LongREST::TCP;
using namespace LongREST::General;

#define BUFFER_SIZE 16384

void ClearBuffer(char* buf, size_t size)
{
	for (int i = 0; i < size; i++) { buf[i] = '\0'; }
}

int main(int argc, char const *argv[])
{
	char buffer[BUFFER_SIZE]; ClearBuffer(buffer, BUFFER_SIZE);
	TcpSocket socket; socket.Setup(); socket.SetAddress(AF_INET, "127.0.0.1", TCP_SERVER_DEFAULT_PORT);
	TcpConnection con;
	string toSend = "GET / HTTP/1.1\r\n";

	if (socket.IsGood())
	{
		con = socket.Connect();
	}
	else
	{
		cout << "Bad socket..." << endl;
	}

	if (con.IsGood())
	{
		con.Send(toSend.c_str(), toSend.length());
		this_thread::sleep_for(chrono::milliseconds(500));
		int outSize = con.Read(buffer, BUFFER_SIZE);

		cout << "Output Size: " << outSize << "\n" 
		<< "Output:\n" << string(buffer) << endl;
	}
	else
	{
		cout << "We got a bad connection..." << endl;
	}

	return 0;
}