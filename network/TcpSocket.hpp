#ifndef TCP_SERVER_HEADER
#define TCP_SERVER_HEADER
#define TCP_SERVER_DEFAULT_PORT 80

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>

#include "TcpConnection.hpp"

class TcpSocket
{
public:
	TcpSocket() {}

	~TcpSocket() { Close(); }

    int Setup()
    {
        return Setup(TCP_SERVER_DEFAULT_PORT);
    }

    int Setup(int port)
    {
        return Setup(AF_INET, port);
    }

    int Setup(int family, int port)
    {
        return Setup(family, SOCK_STREAM, 0, port);
    }

    int Setup(int family, int type, int protocol, int port)
    {
        return Setup(family, type, protocol, INADDR_ANY, port);
    }

    int Setup(int family, int type, int protocol, std::string address, int port)
    {
        in_addr temp;
        inet_aton(address.c_str(), &temp);
        return Setup(family, type, protocol, temp.s_addr, port);
    }

    int Setup(int family, int type, int protocol, unsigned long address, int port)
    {
        SetSocket(family, type, protocol);
        SetAddress(family, address, port);
        Bind();

        return sockfd;
    }

    void Bind()
    {
        // Attach socket
        if (bind(sockfd, (const sockaddr*)&_address, sizeof(_address))<0) 
        {
            // Throw an exception here
        }
    }

    int SetSocket(int family, int type, int protocol)
    {
        sockfd = socket(family, type, protocol);
        return sockfd;
    }

    void SetAddress(int family, unsigned long address, int port)
    {
        // Setup the address
        _address.sin_family = family; 
        _address.sin_addr.s_addr = address; 
        _address.sin_port = htons(port);
    }

    void Listen(int backlog)
    {
        // Acquire a log of connections
        if (listen(sockfd, backlog) < 0)
        {
            // Throw an exception here
        }
    }

	TcpConnection Accept()
	{
		int connection;

		if ((connection = accept(sockfd, (sockaddr*)&_address, (socklen_t*)&addrlen))<0) 
		{
			// If no accepted connection, return a bad connection
			return TcpConnection(-1);
		}
		else
		{
			return TcpConnection(connection);
		}
	}

    void Close()
    {
        close(sockfd);
    }

private:
    int sockfd; // The socket
    sockaddr_in _address; // Our address
    int addrlen = sizeof(_address);
};

#endif