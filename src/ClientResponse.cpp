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

#include "ClientResponse.h"
#include <windows.h>
#include <vector>
#include "clientManager.h"
#include <string>

void ClientResponse_t(std::shared_ptr<ClientsManager> clientManager_ptr) {

	int nDataLength = 0;
	int m_receivingStatus = 0;
	timeval m_timeInterval;
	m_timeInterval.tv_sec = 0;
	m_timeInterval.tv_usec = mysocket::WinSocket::selectInterval_us; // The maximum time for select(..) to wait
	char recv_buf[mysocket::WinSocket::RecieveBuffLength + 1] = {};	// +1 for extra NULL character at the end
	std::string cmd;
	std::string recv_data;
	std::string b64_str_recv;
	std::string decoded_str;
	std::string cmd_b64_str;
	std::string transfer_data;

	while (TRUE) {
		while (clientManager_ptr->getActiveClientsCount() < 1) {		// wait for client(s) to connect
			Sleep(mysocket::WinSocket::listenSleepInterval_ms);
		}
		m_receivingStatus = select(NULL, &clientManager_ptr->m_readFds, NULL, NULL, &m_timeInterval);
		if (m_receivingStatus == 0) {									// No socket active to read
			clientManager_ptr->m_readFds.fd_count = clientManager_ptr->getActiveClientsCount();
			continue;
		}
		if (FD_ISSET(clientManager_ptr->getActiveClientSocket(), &clientManager_ptr->m_readFds)) {
			nDataLength = recv(clientManager_ptr->getActiveClientSocket(), recv_buf, mysocket::WinSocket::RecieveBuffLength, 0);
			if ((nDataLength == SOCKET_ERROR) || (nDataLength == 0)) {
				clientManager_ptr->clientDisconnected();
			}
		}
		for (unsigned int i = 0; i < clientManager_ptr->MaxNumOfClientsSupported; ++i) {
			if (clientManager_ptr->getClientSocket(i) != INVALID_SOCKET) {
				FD_SET(clientManager_ptr->getClientSocket(i), &clientManager_ptr->m_readFds);
			}
		}
		recv_data.append(recv_buf);
		memset(recv_buf, 0, sizeof(recv_buf));
		std::cout << recv_data << std::flush;
		recv_data.clear();
		clientManager_ptr->m_readFds.fd_count = clientManager_ptr->getActiveClientsCount();
	}
}