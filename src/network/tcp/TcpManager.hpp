#ifndef TCP_MANAGER_HEADER
#define TCP_MANAGER_HEADER

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <string>
#include <list>
#include <algorithm>

#include "TcpSocket.hpp"
#include "TcpConnection.hpp"

#define TCP_MANAGER_TEST_BUFFER_SIZE 10

namespace LongREST
{
	namespace TCP
	{
		typedef void (*OnSocketRead)(TcpSocket& socket, TcpConnection& newConnection);
		typedef void (*OnConnectionRead)(TcpConnection&);
		typedef void (*OnStdinRead)();

		class TcpManager
		{
			public:
			TcpManager(bool isServer = true) : _isServer(isServer)
			{

			}

			void Setup()
			{

			}

			void Loop(bool doesIncludeStdin = true, int timeoutMilliseconds = 0)
			{
				timeval timeout = {0, 0};

				if (timeoutMilliseconds != 0)
				{
					timeout.tv_sec = timeoutMilliseconds / 1000;
					timeout.tv_usec = (timeoutMilliseconds % 1000) * 1000;
				}
				
				// Set up our fd_sets...
				FD_ZERO(&fdRead);
				//FD_ZERO(&fdExcept);

				int highestFd = 0;

				if (doesIncludeStdin) 
				{ 
					FD_SET(STDIN_FILENO, &fdRead); 
					if (STDIN_FILENO > highestFd) { highestFd = STDIN_FILENO; }
				}

				if (_isServer) 
				{ 
					int sockfd = socket.GetSockFileDescriptor();
					FD_SET(sockfd, &fdRead); 
					if (sockfd > highestFd) { highestFd = sockfd; }
				}

				for (list<TcpConnection>::iterator i = connections.begin(); i != connections.end(); i++)
				{
					FD_SET(i->GetSockFileDescriptor(), &fdRead);
					if (i->GetSockFileDescriptor() > highestFd) { highestFd = i->GetSockFileDescriptor(); }
				}

				// Select
				select(highestFd+1, &fdRead, nullptr, nullptr, &timeout);

				// Read and Clean
				if (doesIncludeStdin) // stdin
				{
					if (onStdRead != nullptr) { onStdRead(); }
				}

				if (_isServer) // Open a new connection and inform the listener
				{
					TcpConnection conn = socket.Accept();

					if (conn.IsGood())
					{
						connections.push_back(conn);
					}

					if (onSockRead != nullptr) { onSockRead(socket, conn); }
				}

				for (list<TcpConnection>::iterator i = connections.begin(); i != connections.end(); i++) // connections
				{
					if (FD_ISSET(i->GetSockFileDescriptor(), &fdRead)) // Has data / is closing
					{
						if (recv(i->GetSockFileDescriptor(), testBuffer, TCP_MANAGER_TEST_BUFFER_SIZE, MSG_PEEK | MSG_DONTWAIT) == 0) // Is dead
						{
							i->Close();
						}
						else // Is alive
						{
							if (onConnRead != nullptr) { onConnRead(*i); }
						}
					}
				}

				// Clean up dead connections
				connections.remove_if([](const TcpConnection& value)->bool
				{
					return value.IsGood();
				});
			}

			void SetSocketCallback(OnSocketRead callback) { onSockRead = callback; }
			void SetConnectionCallback(OnConnectionRead callback) { onConnRead = callback; }
			void SetStdinCallback(OnStdinRead callback) { onStdRead = callback; }

			private:
			TcpSocket socket;
			fd_set fdRead;
			//fd_set fdExcept;

			list<TcpConnection> connections;
			bool _isServer;
			char testBuffer[TCP_MANAGER_TEST_BUFFER_SIZE];

			// Our events
			OnSocketRead onSockRead = nullptr;
			OnConnectionRead onConnRead = nullptr;
			OnStdinRead onStdRead = nullptr;
		};
	}
}

#endif