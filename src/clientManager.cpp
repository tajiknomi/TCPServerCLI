#include "clientManager.h"
#include <iostream>
#include <string>

std::vector<std::string> ClientsManager::extractValidClients() {
	std::vector<std::string> tmp;
	for (unsigned int i = 0; i < MaxNumOfClientsSupported; ++i) {
		if (clientSockets[i] != INVALID_SOCKET) {
			tmp.push_back(std::to_string(i));
		}
	}
	return tmp;
}

void ClientsManager::printValidClients() {
	std::vector<std::string> validClients{ extractValidClients() };
	if (validClients.empty()) {
		std::cout << "Unfortunately, there are no clients available now" << std::endl;
		return;
	}
	std::string tmp{ "That client ID isn't valid. Please choose from the following active clients: [" };
	for (auto& id : validClients) {
		tmp += id + ",";
	}
	tmp.pop_back();
	tmp += "]";
	std::cout << tmp << std::endl;
}

SOCKET ClientsManager::getActiveClientSocket(void) {
	std::lock_guard<std::mutex> lock(mtx);  // Lock the mutex for the duration of this function
	return clientSockets[clientSelected];
}

void ClientsManager::clientConnected(void) {
	std::lock_guard<std::mutex> lock(mtx);  // Lock the mutex for the duration of this function
	++activeClientCount;
}

SOCKET ClientsManager::getClientSocket(const unsigned int& id) {
	if (id >= MaxNumOfClientsSupported) {
		std::cout << "Selected client should be within range of 0 - " << MaxNumOfClientsSupported << std::endl;
		return INVALID_SOCKET;
	}
	return clientSockets[id];
}

void ClientsManager::setClientSocket(const unsigned int& id, const int& value) {
	clientSockets[id] = value;
}

void ClientsManager::clientDisconnected(void) {
	std::lock_guard<std::mutex> lock(mtx);  // Lock the mutex for the duration of this function
	FD_CLR(clientSockets[clientSelected], &m_readFds);
	closesocket(clientSockets[clientSelected]);
	--activeClientCount;
	clientSockets[clientSelected] = INVALID_SOCKET;
	std::cout << "###################### Client " << getActiveClientID() << " is disconnected ######################" << std::endl;
}

void ClientsManager::setActiveClient(const int& id)
{
	if (id >= MaxNumOfClientsSupported) {
		std::cout << "Selected client should be within range of 0 - " << MaxNumOfClientsSupported << std::endl;
		return;
	}
	else if (getClientSocket(id) == INVALID_SOCKET) {
		printValidClients();
		return;
	}
	clientSelected = id;
	std::cout << "client [" << getActiveClientID() << "] activated" << std::endl;
}

unsigned int ClientsManager::getActiveClientID(void) {
	return clientSelected;
}

unsigned int ClientsManager::getActiveClientsCount(void) {
	std::lock_guard<std::mutex> lock(mtx);  // Lock the mutex for the duration of this function
	return activeClientCount;
}