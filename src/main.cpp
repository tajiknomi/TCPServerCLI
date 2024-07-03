#include "tcpNetworkManager.h" /* Always use this header at the top */
#include <iostream>
#include "ClientResponse.h"
#include <string>

#define NumberOfArgs	2

void displayUsageInstructions(void) {
	// Display program instructions
	std::cout << "=== Use the below commands once client(s) is/are connected ===" << std::endl << std::endl;

	// Display commands
	std::cout << "select client [id]" << std::endl;
	std::cout << "exit\n\n\n\n\n";
}


int main(int argc, char* argv[]) {

	if (argc != NumberOfArgs) {
		std::cerr << "Usage: " << argv[0] << " <port_number>\n";
		return EXIT_FAILURE;
	}

	displayUsageInstructions();
	const std::string port_num(argv[1]);

	if (mysocket::WinSocket::isvalidatePort(port_num) == false) {
		return EXIT_FAILURE;
	}

	const std::string server_ip("0.0.0.0");
	std::cout << "Listening on " << server_ip << ":" << port_num << std::endl;

	/* Create a tcp object */
	mysocket::WinSocket tcp;

	/* Initialize Socket */
	tcp.socket_init(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	/* Listen on <port_num> */
	tcp.listen(server_ip.c_str(), port_num.c_str());

	/* Close socket */
	tcp.socket_cleanup();
	return 0;
}