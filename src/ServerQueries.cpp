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

#include "ServerQueries.h"
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include "clientManager.h"


std::string first_numberstring(std::string const& str)
{
	char const* digits = "0123456789";
	std::size_t const n = str.find_first_of(digits);
	if (n != std::string::npos)
	{
		std::size_t const m = str.find_first_not_of(digits, n);
		return str.substr(n, m != std::string::npos ? m - n : m);
	}
	return std::string();
}

void ServerQueries_t(std::shared_ptr<ClientsManager> clientManager_ptr) {

	std::string input_cmd{ "" };
	std::string cmd_b64_str;
	std::string transfer_data;

	while (TRUE) {

		std::getline(std::cin, input_cmd);

		// Check input command for switching client 
		if (input_cmd.compare(0, 13, "select client") == 0){
			std::string tmp_str = first_numberstring(input_cmd);
			if (!tmp_str.empty()) {									// Check if the user doesn't enter Number
				clientManager_ptr->setActiveClient(stoi(tmp_str));	// If the string contains number, convert to string. Otherwise; don't eval this statement (to avoid exception)			
				continue;
			}
		}
		if (clientManager_ptr->getActiveClientSocket() == INVALID_SOCKET) {
			clientManager_ptr->printValidClients();
			continue;
		}
		input_cmd.append("\n");

		//	Send the packet through socket
		send(clientManager_ptr->getActiveClientSocket(), input_cmd.c_str(), input_cmd.size(), 0);
		transfer_data.clear();

		//	If the send command was "exit", then release the socket
		if (!(input_cmd.compare(0, std::string("exit").length(), std::string("exit")))) {
			closesocket(clientManager_ptr->getActiveClientSocket());
		}
	}
}