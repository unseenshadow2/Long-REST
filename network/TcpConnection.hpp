#ifndef TCP_CON_HEADER
#define TCP_CON_HEADER

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

class TcpConnection
{
public:
	TcpConnection() {}
	TcpConnection(int connection) : _connection(connection) {}

	bool IsGood()
	{
		return _connection >= 0;
	}

	int Read(char* buffer, size_t size)
	{
		return read(_connection, buffer, size);
	}

	int Send(const char* const buffer, size_t size, int flags = 0)
	{
		send(_connection, buffer, size, 0);
	}

    void Close()
	{
		close(_connection);
	}
private:
	int _connection = -1;
};

#include "TcpSocket.hpp"

#endif