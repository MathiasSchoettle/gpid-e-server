#include "udp.h"

int main() {
	std::string ipAddress = "192.168.3.255";  // IP address of the destination
	int port = 42069;                     // Port number of the destination
	std::string message = "Broadcast!";  // Message to send

	// Call the function to send the UDP message
	for(int i = 0; i < 100; i++)
		sendUdpMessage(ipAddress, port, message);

	return 0;
}
