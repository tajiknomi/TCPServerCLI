// Copyright (c) Nouman Tajik [github.com/tajiknomi]
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE. 1 

#include "tcpNetworkManager.h"
#include "ClientResponse.h"
#include "ServerQueries.h"
#include "clientManager.h"
#include <thread>
#include <string>


#define NumOfClientsSupported 50

namespace mysocket {

	/* Error Handlers for winsocket API's */
	void WinSocket::Err_handle_WSAStartup(const int &err_code) {
		if (err_code != 0) {
			switch (err_code)
			{
			case WSASYSNOTREADY:		std::cerr << "WSAStartup() : The underlying network subsystem is not ready for network communication" << std::endl; exit(EXIT_FAILURE);
			case WSAVERNOTSUPPORTED:	std::cerr << "WSAStartup() : The version of Windows Sockets support requested is not provided by this particular Windows Sockets implementation" << std::endl; exit(EXIT_FAILURE);
			case WSAEINPROGRESS:		std::cerr << "WSAStartup() : A blocking Windows Sockets 1.1 operation is in progress" << std::endl; exit(EXIT_FAILURE);
			case WSAEPROCLIM:			std::cerr << "WSAStartup() : A limit on the number of tasks supported by the Windows Sockets implementation has been reached" << std::endl; exit(EXIT_FAILURE);
			case WSAEFAULT:				std::cerr << "WSAStartup() : WSAStartup error : The lpWSAData parameter is not a valid pointer" << std::endl; exit(EXIT_FAILURE);
			default:					std::cerr << "WSAStartup() : This error code doesn't exist" << std::endl; exit(EXIT_FAILURE);
			}
		}
	}
	void WinSocket::Err_handle_getaddrinfo(const int &err_code) {
		if (err_code != 0) {
			switch (err_code)
			{
			case EAI_AGAIN:		std::cerr << "getaddrinfo() : A temporary failure in name resolution occurred" << std::endl; exit(EXIT_FAILURE);
			case EAI_BADFLAGS:	std::cerr << "getaddrinfo() : An invalid value was provided for the ai_flags member of the pHints parameter" << std::endl; exit(EXIT_FAILURE);
			case EAI_FAIL:		std::cerr << "getaddrinfo() : A nonrecoverable failure in name resolution occurred" << std::endl; exit(EXIT_FAILURE);
			case EAI_FAMILY:	std::cerr << "getaddrinfo() : The ai_family member of the pHints parameter is not supported" << std::endl; exit(EXIT_FAILURE);
			case EAI_MEMORY:	std::cerr << "getaddrinfo() : A memory allocation failure occurred" << std::endl; exit(EXIT_FAILURE);
			case EAI_NONAME:	std::cerr << "getaddrinfo() : The name does not resolve for the supplied parameters or the pNodeName and pServiceName parameters were not provided" << std::endl; exit(EXIT_FAILURE);
			case EAI_SERVICE:	std::cerr << "getaddrinfo() : The pServiceName parameter is not supported for the specified ai_socktype member of the pHints parameter" << std::endl; exit(EXIT_FAILURE);
			case EAI_SOCKTYPE:	std::cerr << "getaddrinfo() : The ai_socktype member of the pHints parameter is not supported" << std::endl; exit(EXIT_FAILURE);
			default:			std::cerr << "getaddrinfo() : This error code doesn't exist" << std::endl; exit(EXIT_FAILURE);
			}
		}
	}
	void WinSocket::Err_handle_socket(const SOCKET &ret_val) {
		if (ret_val == INVALID_SOCKET)
		{
			const int &err_code = WSAGetLastError();
			switch (err_code)
			{
			case WSANOTINITIALISED:		std::cerr << "socket() : A 0ful WSAStartup call must occur before using this function" << std::endl; exit(EXIT_FAILURE);
			case WSAENETDOWN:			std::cerr << "socket() : The network subsystem or the associated service provider has failed" << std::endl; exit(EXIT_FAILURE);
			case WSAEAFNOSUPPORT:		std::cerr << "socket() : The specified address family is not supported. For example, an application tried to create a socket for the AF_IRDA address family but an infrared adapter and device driver is not installed on the local computer" << std::endl; exit(EXIT_FAILURE);
			case WSAEINPROGRESS:		std::cerr << "socket() : A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function" << std::endl; exit(EXIT_FAILURE);
			case WSAEMFILE:				std::cerr << "socket() : No more socket descriptors are available" << std::endl; exit(EXIT_FAILURE);
			case WSAEINVAL:				std::cerr << "socket() : An invalid argument was supplied. This error is returned if the af parameter is set to AF_UNSPEC and the type and protocol parameter are unspecified" << std::endl; exit(EXIT_FAILURE);
			case WSAEINVALIDPROVIDER:	std::cerr << "socket() : The service provider returned a version other than 2.2" << std::endl; exit(EXIT_FAILURE);
			case WSAEINVALIDPROCTABLE:	std::cerr << "socket() : The service provider returned an invalid or incomplete procedure table to the WSPStartup" << std::endl; exit(EXIT_FAILURE);
			case WSAENOBUFS:			std::cerr << "socket() : No buffer space is available. The socket cannot be created" << std::endl; exit(EXIT_FAILURE);
			case WSAEPROTONOSUPPORT:	std::cerr << "socket() : The specified protocol is not supported" << std::endl; exit(EXIT_FAILURE);
			case WSAEPROTOTYPE:			std::cerr << "socket() : The specified protocol is the wrong type for this socket" << std::endl; exit(EXIT_FAILURE);
			case WSAEPROVIDERFAILEDINIT:std::cerr << "socket() : The service provider failed to initialize. This error is returned if a layered service provider (LSP) or namespace provider was improperly installed or the provider fails to operate correctly" << std::endl; exit(EXIT_FAILURE);
			case WSAESOCKTNOSUPPORT:	std::cerr << "socket() : The specified socket type is not supported in this address family" << std::endl; exit(EXIT_FAILURE);
			default:					std::cerr << "socket() : This error code doesn't exist" << std::endl; exit(EXIT_FAILURE);
			}
		}
	}
	void WinSocket::Err_handle_WSASocket(const SOCKET &ret_val) {
		if (ret_val == INVALID_SOCKET) {

			const int &err_code = WSAGetLastError();
			switch (err_code)
			{
			case WSANOTINITIALISED:			std::cerr << "WSASocket() returns error code : WSANOTINITIALISED" << std::endl;	exit(EXIT_FAILURE);
			case WSAENETDOWN:				std::cerr << "WSASocket() returns error code : WSAENETDOWN" << std::endl;			exit(EXIT_FAILURE);
			case WSAEAFNOSUPPORT:			std::cerr << "WSASocket() returns error code : WSAEAFNOSUPPORT" << std::endl;		exit(EXIT_FAILURE);
			case WSAEFAULT:					std::cerr << "WSASocket() returns error code : WSAEFAULT" << std::endl;			exit(EXIT_FAILURE);
			case WSAEINPROGRESS:			std::cerr << "WSASocket() returns error code : WSAEINPROGRESS" << std::endl;		exit(EXIT_FAILURE);
			case WSAEINVAL:					std::cerr << "WSASocket() returns error code : WSAEINVAL" << std::endl;			exit(EXIT_FAILURE);
			case WSAEINVALIDPROVIDER:		std::cerr << "WSASocket() returns error code : WSAEINVALIDPROVIDER" << std::endl;  exit(EXIT_FAILURE);
			case WSAEINVALIDPROCTABLE:		std::cerr << "WSASocket() returns error code : WSAEINVALIDPROCTABLE" << std::endl; exit(EXIT_FAILURE);
			case WSAEMFILE:					std::cerr << "WSASocket() returns error code : WSAEMFILE" << std::endl;			exit(EXIT_FAILURE);
			case WSAENOBUFS:				std::cerr << "WSASocket() returns error code : WSAENOBUFS" << std::endl;			exit(EXIT_FAILURE);
			case WSAEPROTONOSUPPORT:		std::cerr << "WSASocket() returns error code : WSAEPROTONOSUPPORT" << std::endl;	exit(EXIT_FAILURE);
			case WSAEPROTOTYPE:				std::cerr << "WSASocket() returns error code : WSAEPROTOTYPE" << std::endl;		exit(EXIT_FAILURE);
			case WSAEPROVIDERFAILEDINIT:	std::cerr << "WSASocket() returns error code : WSAEPROVIDERFAILEDINIT" << std::endl; exit(EXIT_FAILURE);
			case WSAESOCKTNOSUPPORT:		std::cerr << "WSASocket() returns error code : WSAESOCKTNOSUPPORT" << std::endl;	 exit(EXIT_FAILURE);
			}
		}
	}
	void WinSocket::Err_handle_send(const int &ret_val) {
		if (ret_val == SOCKET_ERROR)
		{
			const int &err_code = WSAGetLastError();
			switch (err_code)
			{
			case WSANOTINITIALISED:	std::cerr << "send() : A 0ful WSAStartup call must occur before using this function" << std::endl; exit(EXIT_FAILURE);
			case WSAENETDOWN:		std::cerr << "send() : The network subsystem has failed" << std::endl; exit(EXIT_FAILURE);
			case WSAEACCES:			std::cerr << "send() : The requested address is a broadcast address, but the appropriate flag was not set. Call setsockopt with the SO_BROADCAST socket option to enable use of the broadcast address" << std::endl; exit(EXIT_FAILURE);
			case WSAEINTR:			std::cerr << "send() : A blocking Windows Sockets 1.1 call was canceled through WSACancelBlockingCall" << std::endl; exit(EXIT_FAILURE);
			case WSAEINPROGRESS:	std::cerr << "send() : A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function" << std::endl; exit(EXIT_FAILURE);
			case WSAEFAULT:			std::cerr << "send() : The buf parameter is not completely contained in a valid part of the user address space" << std::endl; exit(EXIT_FAILURE);
			case WSAENETRESET:		std::cerr << "send() : The connection has been broken due to the keep-alive activity detecting a failure while the operation was in progress" << std::endl; exit(EXIT_FAILURE);
			case WSAENOBUFS:		std::cerr << "send() : No buffer space is available" << std::endl; exit(EXIT_FAILURE);
			case WSAENOTCONN:		std::cerr << "send() : The socket is not connected" << std::endl; exit(EXIT_FAILURE);
			case WSAENOTSOCK:		std::cerr << "send() : The descriptor is not a socket" << std::endl; exit(EXIT_FAILURE);
			case WSAEOPNOTSUPP:		std::cerr << "send() : MSG_OOB was specified, but the socket is not stream-style such as type SOCK_STREAM, OOB data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only receive operations" << std::endl; exit(EXIT_FAILURE);
			case WSAESHUTDOWN:		std::cerr << "send() : The socket has been shut down; it is not possible to send on a socket after shutdown has been invoked with how set to SD_SEND or SD_BOTH" << std::endl; exit(EXIT_FAILURE);
			case WSAEWOULDBLOCK:	std::cerr << "send() : The socket is marked as nonblocking and the requested operation would block" << std::endl; exit(EXIT_FAILURE);
			case WSAEMSGSIZE:		std::cerr << "send() : The socket is message oriented, and the message is larger than the maximum supported by the underlying transport" << std::endl; exit(EXIT_FAILURE);
			case WSAEHOSTUNREACH:	std::cerr << "send() : The remote host cannot be reached from this host at this time" << std::endl; exit(EXIT_FAILURE);
			case WSAEINVAL:			std::cerr << "send() : The socket has not been bound with bind, or an unknown flag was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled" << std::endl; exit(EXIT_FAILURE);
			case WSAECONNABORTED:	std::cerr << "send() : The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable" << std::endl; exit(EXIT_FAILURE);
			case WSAECONNRESET:		std::cerr << "send() : The virtual circuit was reset by the remote side executing a hard or abortive close. For UDP sockets, the remote host was unable to deliver a previously sent UDP datagram and responded with a \"Port Unreachable\" ICMP packet. The application should close the socket as it is no longer usable" << std::endl; exit(EXIT_FAILURE);
			case WSAETIMEDOUT:		std::cerr << "send() : The connection has been dropped, because of a network failure or because the system on the other end went down without notice" << std::endl; exit(EXIT_FAILURE);
			default:				std::cerr << "send() : This error code doesn't exist" << std::endl; exit(EXIT_FAILURE);
			}

		}
	}
	void WinSocket::Err_handle_shutdown(const int &ret_val) {
		if (ret_val == SOCKET_ERROR)
		{
			const int &err_code = WSAGetLastError();
			switch (err_code) {
			case WSAECONNABORTED:	std::cerr << "shutdown() : The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable" << std::endl; exit(EXIT_FAILURE);
			case WSAECONNRESET:		std::cerr << "shutdown() : The virtual circuit was reset by the remote side executing a hard or abortive close. The application should close the socket as it is no longer usable" << std::endl; exit(EXIT_FAILURE);
			case WSAEINPROGRESS:	std::cerr << "shutdown() : A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function" << std::endl; exit(EXIT_FAILURE);
			case WSAEINVAL:			std::cerr << "shutdown() : The how parameter is not valid, or is not consistent with the socket type. For example, SD_SEND is used with a UNI_RECV socket type" << std::endl; exit(EXIT_FAILURE);
			case WSAENETDOWN:		std::cerr << "shutdown() : The network subsystem has failed" << std::endl; exit(EXIT_FAILURE);
			case WSAENOTCONN:		std::cerr << "shutdown() : The socket is not connected. This error applies only to a connection-oriented socket" << std::endl; exit(EXIT_FAILURE);
			case WSAENOTSOCK:		std::cerr << "shutdown() : The descriptor is not a socket" << std::endl; exit(EXIT_FAILURE);
			case WSANOTINITIALISED:	std::cerr << "shutdown() : A 0ful WSAStartup call must occur before using this function" << std::endl; exit(EXIT_FAILURE);
			default:				std::cerr << "shutdown() : This error code doesn't exist" << std::endl; exit(EXIT_FAILURE);
			}
		}
	}
	void WinSocket::Err_handle_recv(const int &ret_val) {
		if (ret_val == SOCKET_ERROR) {
			const int &err_code = WSAGetLastError();
			switch (err_code) {
			case WSANOTINITIALISED:	std::cerr << "recv() : A 0ful WSAStartup call must occur before using this function" << std::endl; exit(EXIT_FAILURE);
			case WSAENETDOWN:		std::cerr << "recv() : The network subsystem has failed" << std::endl; exit(EXIT_FAILURE);
			case WSAEFAULT:			std::cerr << "recv() : The buf parameter is not completely contained in a valid part of the user address space" << std::endl; exit(EXIT_FAILURE);
			case WSAENOTCONN:		std::cerr << "recv() : The socket is not connected" << std::endl; exit(EXIT_FAILURE);
			case WSAEINTR:			std::cerr << "recv() : The (blocking) call was canceled through WSACancelBlockingCall" << std::endl; exit(EXIT_FAILURE);
			case WSAEINPROGRESS:	std::cerr << "recv() : A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function" << std::endl; exit(EXIT_FAILURE);
			case WSAENETRESET:		std::cerr << "recv() : For a connection-oriented socket, this error indicates that the connection has been broken due to keep-alive activity that detected a failure while the operation was in progress. For a datagram socket, this error indicates that the time to live has expired" << std::endl; exit(EXIT_FAILURE);
			case WSAENOTSOCK:		std::cerr << "recv() : The descriptor is not a socket" << std::endl; exit(EXIT_FAILURE);
			case WSAEOPNOTSUPP:		std::cerr << "recv() : MSG_OOB was specified, but the socket is not stream-style such as type SOCK_STREAM, OOB data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only send operations" << std::endl; exit(EXIT_FAILURE);
			case WSAESHUTDOWN:		std::cerr << "recv() : The socket has been shut down; it is not possible to receive on a socket after shutdown has been invoked with how set to SD_RECEIVE or SD_BOTH" << std::endl; exit(EXIT_FAILURE);
			case WSAEWOULDBLOCK:	std::cerr << "recv() : The socket is marked as nonblocking and the receive operation would block" << std::endl; exit(EXIT_FAILURE);
			case WSAEMSGSIZE:		std::cerr << "recv() : The message was too large to fit into the specified buffer and was truncated" << std::endl; exit(EXIT_FAILURE);
			case WSAEINVAL:			std::cerr << "recv() : The socket has not been bound with bind, or an unknown flag was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled or (for byte stream sockets only) len was zero or negative" << std::endl; exit(EXIT_FAILURE);
			case WSAECONNABORTED:	std::cerr << "recv() : The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable" << std::endl; exit(EXIT_FAILURE);
			case WSAETIMEDOUT:		std::cerr << "recv() : The connection has been dropped because of a network failure or because the peer system failed to respond" << std::endl; exit(EXIT_FAILURE);
			case WSAECONNRESET:		std::cerr << "recv() : The virtual circuit was reset by the remote side executing a hard or abortive close. The application should close the socket as it is no longer usable. On a UDP-datagram socket, this error would indicate that a previous send operation resulted in an ICMP \"Port Unreachable\" message" << std::endl;
			default:				std::cerr << "recv() : This error code doesn't exist" << std::endl; exit(EXIT_FAILURE);
			}
		}
	}
	void WinSocket::Err_handle_bind(const int &ret_val) {
		if (ret_val == SOCKET_ERROR) {
			const int &err_code = WSAGetLastError();
			switch (err_code) {
			case WSANOTINITIALISED:	std::cerr << "bind() returns error code : WSANOTINITIALISED" << std::endl; exit(EXIT_FAILURE);
			case WSAENETDOWN:		std::cerr << "bind() returns error code : WSAENETDOWN" << std::endl;		exit(EXIT_FAILURE);
			case WSAEACCES:			std::cerr << "bind() returns error code : WSAEACCES" << std::endl;			exit(EXIT_FAILURE);
			case WSAEADDRINUSE:		std::cerr << "bind() returns error code : WSAEADDRINUSE" << std::endl;		exit(EXIT_FAILURE);
			case WSAEADDRNOTAVAIL:	std::cerr << "bind() returns error code : WSAEADDRNOTAVAIL" << std::endl;	exit(EXIT_FAILURE);
			case WSAEFAULT:			std::cerr << "bind() returns error code : WSAEFAULT" << std::endl;			exit(EXIT_FAILURE);
			case WSAEINPROGRESS:	std::cerr << "bind() returns error code : WSAEINPROGRESS" << std::endl;	exit(EXIT_FAILURE);
			case WSAEINVAL:			std::cerr << "bind() returns error code : WSAEINVAL" << std::endl;			exit(EXIT_FAILURE);
			case WSAENOBUFS:		std::cerr << "bind() returns error code : WSAENOBUFS" << std::endl;		exit(EXIT_FAILURE);
			case WSAENOTSOCK:		std::cerr << "bind() returns error code : WSAENOTSOCK" << std::endl;		exit(EXIT_FAILURE);
			}
		}
	}
	void WinSocket::Err_handle_listen(const int &ret_val) {
		if (ret_val == SOCKET_ERROR) {
			const int &err_code = WSAGetLastError();
			switch (err_code) {
			case WSANOTINITIALISED:	std::cerr << "listen() returns error code : WSANOTINITIALISED" << std::endl;	exit(EXIT_FAILURE);
			case WSAENETDOWN:		std::cerr << "listen() returns error code : WSAENETDOWN" << std::endl;			exit(EXIT_FAILURE);
			case WSAEADDRINUSE:		std::cerr << "listen() returns error code : WSAEADDRINUSE" << std::endl;		exit(EXIT_FAILURE);
			case WSAEINPROGRESS:	std::cerr << "listen() returns error code : WSAEINPROGRESS" << std::endl;		exit(EXIT_FAILURE);
			case WSAEINVAL:			std::cerr << "listen() returns error code : WSAEINVAL" << std::endl;			exit(EXIT_FAILURE);
			case WSAEISCONN:		std::cerr << "listen() returns error code : WSAEISCONN" << std::endl;			exit(EXIT_FAILURE);
			case WSAEMFILE:			std::cerr << "listen() returns error code : WSAEMFILE" << std::endl;			exit(EXIT_FAILURE);
			case WSAENOBUFS:		std::cerr << "listen() returns error code : WSAENOBUFS" << std::endl;			exit(EXIT_FAILURE);
			case WSAENOTSOCK:		std::cerr << "listen() returns error code : WSAENOTSOCK" << std::endl;			exit(EXIT_FAILURE);
			case WSAEOPNOTSUPP:		std::cerr << "listen() returns error code : WSAEOPNOTSUPP" << std::endl;		exit(EXIT_FAILURE);
			}
		}
	}
	void WinSocket::Err_handle_inet_pton(const int &ret_val){
		if (ret_val != 1){
			int val = WSAGetLastError();
			if (val == WSAEAFNOSUPPORT){
				std::cout << "inet_pton() return error code : WSAEAFNOSUPPORT\n" << std::endl;
			}
			else{
				std::cout << "inet_pton() return error code : WSAEFAULT\n" << std::endl;
			}
			exit(EXIT_FAILURE);
		}
	}
	void WinSocket::Err_handle_accept(const int &ret_val) {
		if (ret_val == SOCKET_ERROR) {
			const int &err_code = WSAGetLastError();
			switch (err_code){
			case WSANOTINITIALISED:	std::cerr << "accept() returns error code : WSANOTINITIALISED" << std::endl;	exit(EXIT_FAILURE);
			case WSAECONNRESET:		std::cerr << "accept() returns error code : WSAECONNRESET" << std::endl;		exit(EXIT_FAILURE);
			case WSAEFAULT:			std::cerr << "accept() returns error code : WSAEFAULT" << std::endl;			exit(EXIT_FAILURE);
			case WSAEINTR:			std::cerr << "accept() returns error code : WSAEINTR" << std::endl;			exit(EXIT_FAILURE);
			case WSAEINVAL:			std::cerr << "accept() returns error code : WSAEINVAL" << std::endl;			exit(EXIT_FAILURE);
			case WSAEINPROGRESS:	std::cerr << "accept() returns error code : WSAEINPROGRESS" << std::endl;		exit(EXIT_FAILURE);
			case WSAEMFILE:			std::cerr << "accept() returns error code : WSAEMFILE" << std::endl;			exit(EXIT_FAILURE);
			case WSAENETDOWN:		std::cerr << "accept() returns error code : WSAENETDOWN" << std::endl;			exit(EXIT_FAILURE);
			case WSAENOBUFS:		std::cerr << "accept() returns error code : WSAENOBUFS" << std::endl;			exit(EXIT_FAILURE);
			case WSAENOTSOCK:		std::cerr << "accept() returns error code : WSAENOTSOCK" << std::endl;			exit(EXIT_FAILURE);
			case WSAEOPNOTSUPP:		std::cerr << "accept() returns error code : WSAEOPNOTSUPP" << std::endl;		exit(EXIT_FAILURE);
			case WSAEWOULDBLOCK:	std::cerr << "accept() returns error code : WSAEWOULDBLOCK" << std::endl;		exit(EXIT_FAILURE);
			}
		}
	}

	/*	Static function(s)	*/
	bool WinSocket::isvalidatePort(const std::string& input)
	{
		for (char c : input) {
			if (!std::isdigit(c)) {
				std::cerr << "PORT is invalid, please put only the number for PORT [1 - 65535]\n";
				return false;
			}
		}
		try {
			int port = std::stoi(input);
			if (port < 1 || port > 65535) {
				std::cerr << "PORT is invalid, please select a PORT in the range [1 - 65535]\n";
				return false;
			}
		}
		catch (const std::invalid_argument&) {
			std::cerr << "PORT is invalid, please enter a valid number in the range [1 - 65535]\n";
			return false;
		}
		catch (const std::out_of_range&) {
			std::cerr << "PORT is invalid, number out of range\n";
			return false;
		}
		return true;
	}

	/* API's */
	void WinSocket::socket_init(const int& specify_address_family, const int& specify_socket_type, const int& specify_protocol)
	{
		Err_handle_WSAStartup(WSAStartup(MAKEWORD(2, 2), &wsaData));
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = specify_address_family;
		hints.ai_socktype = specify_socket_type;
		hints.ai_protocol = specify_protocol;
	}
	void WinSocket::connect(const PCSTR& IP_address, const PCSTR& port_num)
	{
		/* Resolve the server address and port */
		Err_handle_getaddrinfo(getaddrinfo(IP_address, port_num, &hints, &result));
		ConnectSocket = INVALID_SOCKET;

		/* Attempt to connect to the first address returned by the call to getaddrinfo */
		ptr = result;

		/* The "windows_socket_init_args_ptr->result" may contain more than one IpAddr you can connect to */
		/* If the client is unable to connect to the first addr, it can try a list of ipAddr in "windows_socket_init_args_ptr->result" */
		/* See https://docs.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-getaddrinfo for the detail */

		/* Create a SOCKET for connecting to server */

		/* 1) For Asynchronous I/O, Overlapped operations which allow multiple operations to be initiated and in progress simultaneously */
//		ConnectSocket = socket(hints.ai_family, hints.ai_socktype, hints.ai_protocol); 

		/* 2) For Synchronous I/O, Non-Overlapped operations */
		ConnectSocket = WSASocket(hints.ai_family, hints.ai_socktype, hints.ai_protocol, NULL, 0, 0);   /* Used because Asynchronous/Overlapped socket cannot be use for SOCKET--I/O Redirection */
		Err_handle_socket(ConnectSocket);

		int iResult = SOCKET_ERROR;

		while (TRUE) {
			/* Connect to server */
			iResult = ::connect(ConnectSocket, ptr->ai_addr, ptr->ai_addrlen);
			if (iResult == SOCKET_ERROR) {
				Sleep(connectDelay_ms);			/* Time interval between connection tries */
				continue;
			}
			break;						/* Once data is recieved, Leave the loop */
		}
	}
	void WinSocket::listen(const PCSTR& IP_address, const PCSTR& port_num) {

		// This pointer will be shared with sending and recieving threads (i.e. ClientResponse and ServerQueries modules)
		std::shared_ptr<ClientsManager> clientManager_ptr = std::make_shared<ClientsManager>();
	
		sockaddr_in service = {};
		service.sin_family = hints.ai_family;
		Err_handle_inet_pton(inet_pton(AF_INET, IP_address, (PVOID)&service.sin_addr));	
		u_short port_number = atoi(port_num);
		service.sin_port = htons(port_number);

		// 1) For Asynchronous I/O, Overlapped operations which allow multiple operations to be initiated and in progress simultaneously 
		ListenSocket = socket(hints.ai_family, hints.ai_socktype, hints.ai_protocol);

//		Used because Asynchronous / Overlapped socket cannot be use for SOCKET--I / O Redirection
//		ListenSocket = WSASocket(hints.ai_family, hints.ai_socktype, hints.ai_protocol, NULL, 0, 0);
		Err_handle_WSASocket(ListenSocket);
		Err_handle_bind(bind(ListenSocket, (SOCKADDR*)&service, sizeof(service)));

		// Setting up NON-BLOCKING mode for socket 
		u_long iMode = 1;
		int iResult = ioctlsocket(ListenSocket, FIONBIO, &iMode);
		if (iResult != NO_ERROR)
			std::cout << "ioctlsocket failed with error: %ld\n" <<  iResult;

//		Listen for incoming connection requests on the created socket
		Err_handle_listen(::listen(ListenSocket, 1)); 
		
//		Create a SOCKET for Accepting incoming requests
		std::cout << "Waiting for client(s) to connect...\n" << std::endl;
		int err_code;
		
		std::thread send_thread(ServerQueries_t, clientManager_ptr);					// A thread which send commands to client(s)
		std::thread recv_tread(ClientResponse_t, clientManager_ptr);					// A thread which recieves response/data from client(s) 

		for (unsigned int ClientId = 0; ClientId < NumOfClientsSupported; ++ClientId) {
//		Wait of incoming connection
			do{
				Sleep(listenSleepInterval_ms);
				err_code = 0;
				AcceptSocket = accept(ListenSocket, NULL, NULL);
				if (AcceptSocket == SOCKET_ERROR) {
					err_code = WSAGetLastError();
				}	
			} while (err_code == WSAEWOULDBLOCK);
	
			FD_SET(AcceptSocket, &clientManager_ptr->m_readFds);	// Add new socket to socket-pool
			clientManager_ptr->clientConnected();
			std::cout << "====================== Client " << ClientId << " is connected ======================" << std::endl;

			clientManager_ptr->setClientSocket(ClientId, AcceptSocket);
//			Sleep(500);							/* Wait before starting listening to next client */
			AcceptSocket = INVALID_SOCKET;
		}
		send_thread.join();
		recv_tread.join();
	}
	void WinSocket::transmit (const char* transmit_buffer, const int& data_size_in_bytes) {
		WinSocket::Err_handle_send(send(AcceptSocket, transmit_buffer, data_size_in_bytes, 0));			
	}
	void WinSocket::socket_close() {
		closesocket(ConnectSocket);
		closesocket(ListenSocket);
		closesocket(AcceptSocket);
	}
	void WinSocket::socket_cleanup()
	{
		freeaddrinfo(result);
		closesocket(ConnectSocket);
		closesocket(ListenSocket);
		closesocket(AcceptSocket);
		WSACleanup();
	}
}