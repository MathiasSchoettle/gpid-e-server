#include "udp.h"

int main() {
	std::string ipAddress = "192.168.3.255";  // IP address of the destination
	int port = GPID_E_PORT;                  // Port number of the destination
	std::string message = "Broadcast!";  // Message to send

	// Call the function to send the UDP message
	// for(int i = 0; i < 100; i++)
	// 	sendUdpMessage(ipAddress, port, message);

	awaitResponse();
	return 0;
}
