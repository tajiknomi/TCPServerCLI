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