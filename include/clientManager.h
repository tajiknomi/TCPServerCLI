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

#include <Windows.h>
#include <vector>
#include <mutex>

class ClientsManager {

private:
	std::mutex mtx;
	unsigned int clientSelected;
	unsigned int activeClientCount;
	std::vector<SOCKET> clientSockets;

public:
	static const unsigned int MaxNumOfClientsSupported{ 100 };		
	FD_SET m_readFds;
	
	// CONSTRUCTOR
	ClientsManager() : clientSockets(MaxNumOfClientsSupported, INVALID_SOCKET), clientSelected(0), activeClientCount(0) { FD_ZERO(&m_readFds); };

public:		// Public Methods	
	unsigned int getActiveClientsCount(void);
	SOCKET getClientSocket(const unsigned int& clientID);
	unsigned int getActiveClientID(void);
	SOCKET getActiveClientSocket(void);
	void setActiveClient(const int& clientID);
	void setClientSocket(const unsigned int& clientID, const int& value);	
	void clientConnected(void);
	void clientDisconnected(void);
	std::vector<std::string> extractValidClients();
	void printValidClients();
};