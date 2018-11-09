#ifndef TCP_CON_HEADER
#define TCP_CON_HEADER

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

namespace LongREST
{
	namespace TCP
	{
		class TcpConnection
		{
		public:
			TcpConnection() {}
			TcpConnection(int connection) : _connection(connection) {}

			const bool IsGood() const
			{
				return _connection >= 0;
			}

			int Read(char* buffer, size_t size)
			{
				return read(_connection, buffer, size);
			}

			int Send(const char* const buffer, size_t size, int flags = 0)
			{
				send(_connection, buffer, size, flags);
			}

			const int GetSockFileDescriptor() const { return _connection; }

			void Close()
			{
				close(_connection);
				_connection = -1;
			}

			bool operator==(const TcpConnection &rhs) { return _connection == rhs._connection; }
			bool operator!=(const TcpConnection &rhs) { return _connection != rhs._connection; }
			bool operator<(const TcpConnection &rhs) { return _connection < rhs._connection; }
			bool operator>(const TcpConnection &rhs) { return _connection > rhs._connection; }
			bool operator<=(const TcpConnection &rhs) { return _connection <= rhs._connection; }
			bool operator>=(const TcpConnection &rhs) { return _connection >= rhs._connection; }

		private:
			int _connection = -1;
		};
	}
}
#include "TcpSocket.hpp"

#endif