#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

bool sendUdpMessage(const std::string& ipAddress, int port, const std::string& message) {
	// Create a UDP socket
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		std::cerr << "Failed to create socket." << std::endl;
		return false;
	}

	// Set up the destination address
	sockaddr_in destAddr{};
	destAddr.sin_family = AF_INET;
	destAddr.sin_port = htons(port);
	if (inet_pton(AF_INET, ipAddress.c_str(), &(destAddr.sin_addr)) <= 0) {
		std::cerr << "Invalid IP address." << std::endl;
		close(sockfd);
		return false;
	}

	// Send the UDP message
	ssize_t sentBytes = sendto(sockfd, message.c_str(), message.size(), 0,
							   reinterpret_cast<struct sockaddr*>(&destAddr), sizeof(destAddr));
	if (sentBytes < 0) {
		std::cerr << "Failed to send UDP message." << std::endl;
		close(sockfd);
		return false;
	}

	std::cout << "UDP message sent successfully." << std::endl;

	// Close the socket
	close(sockfd);
	return true;
}

int main() {
	std::string ipAddress = "10.8.6.126";  // IP address of the destination
	int port = 42069;                     // Port number of the destination
	std::string message = "Hello, UDP!";  // Message to send

	// Call the function to send the UDP message
	sendUdpMessage(ipAddress, port, message);

	return 0;
}
