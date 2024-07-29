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

#pragma once
//#pragma comment(lib, "Ws2_32.lib") /* Linking against the Ws2_32.lib library */

/* This macro skip (excludes) APIs such as Cryptography, DDE, RPC, Shell, and Windows Sockets in <Windows.h> while compilation [To speed the build process] */
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>


enum class ConnectionBehaviour : unsigned int {
	DropConnection,
	KeepAlive,
	WaitForPeerToDropConnection
};

namespace mysocket {

	class WinSocket {

	private:
		WSADATA wsaData;
		int address_family;
		int socket_type;
		int protocol;
		struct addrinfo hints;
		struct addrinfo* result;
		struct addrinfo* ptr;

	private:
		/* Error handlers for windows socket API's */
		void Err_handle_WSAStartup(const int& err_code);
		void Err_handle_getaddrinfo(const int& err_code);
		void Err_handle_socket(const SOCKET& ret_val);
		void Err_handle_WSASocket(const SOCKET& ret_val);
		void Err_handle_send(const int& ret_val);
		void Err_handle_shutdown(const int& ret_val);
		void Err_handle_recv(const int& ret_val);
		void Err_handle_bind(const int& ret_val);
		void Err_handle_inet_pton(const int& ret_val);
		void Err_handle_listen(const int& ret_val);
		void Err_handle_accept(const int& ret_val);

	public:
		SOCKET ConnectSocket;
		SOCKET ListenSocket;
		SOCKET AcceptSocket;
		static const unsigned int RecieveBuffLength{ 4096 };		// 4kb
		static const unsigned int connectDelay_ms{ 4000 };			// used in bind mode i.e. connect(..)
		static const unsigned int listenSleepInterval_ms{ 500 };	// used in reverse connection i.e. listen(..)
		static const unsigned int selectInterval_us { 400000 };		// used in select(..) api

		/* Constructor */
		WinSocket() : ConnectSocket(INVALID_SOCKET),
					  ListenSocket(INVALID_SOCKET), 
					  AcceptSocket(INVALID_SOCKET), 
					  result(NULL),
					  ptr(NULL),
					  address_family(0),
					  protocol(0), 
					  socket_type(SOCK_STREAM) { 
		
			hints	=		{ NULL };
			wsaData =		{ NULL };
		
		};

	public:			/* Static function(s) */
		static bool isvalidatePort(const std::string& input);

	public:	/* Public API's */	
		void socket_init(const int& specify_address_family, const int& specify_socket_type, const int& specify_protocol);
		void connect(const PCSTR& IP_address, const PCSTR& port_num);
		void listen(const PCSTR& IP_address, const PCSTR& port_num);
		void transmit(const char* transmit_buffer, const int& data_size_in_bytes);
		void socket_close();			/* This closes the socket but DOES NOT release the internal resources */
		void socket_cleanup();			/* Close the socket and release the resources */

		
	public:		/*	Destructor	*/ 
		virtual ~WinSocket() { socket_cleanup(); }		/* In case if user forget to cleanup or exception caught */
	};
}