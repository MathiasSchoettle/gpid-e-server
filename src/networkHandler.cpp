#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ether.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "networkHandler.h"

bool scan(const std::string& ipAddress, int port, const std::string& message) {
	// Create a UDP socket
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    int broadcast = 1;
	if (sockfd < 0) {
		std::cerr << "Failed to create socket." << std::endl;
		return false;
	}

	// Set up the destination address
	sockaddr_in destAddr{};
	destAddr.sin_family = AF_INET;
	destAddr.sin_port = htons(port);
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST,
                   &broadcast, sizeof broadcast) < 0)
        printf("setsockopt");

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



int wait_for_clients(){
 // Create a socket
    int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    // Enable broadcast
    int broadcastEnable = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) < 0) {
        std::cerr << "Failed to enable broadcast." << std::endl;
        close(serverSocket);
        return 1;
    }

    // Bind the socket to a specific port
    int port = GPID_E_PORT;  // Change this to the desired port number
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Failed to bind socket to port " << port << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "Server is listening for UDP broadcast on port " << port << std::endl;

    // Receive incoming packets
    char buffer[BUFFER_SIZE];
    sockaddr_in clientAddress{};
    socklen_t clientAddressLength = sizeof(clientAddress);

    while (true) {
        // Receive packet
        int bytesRead = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&clientAddress, &clientAddressLength);
        if (bytesRead < 0) {
            std::cerr << "Failed to receive packet" << std::endl;
            close(serverSocket);
            return 1;
        }

        
        // Retrieve source IP address
        std::string sourceIP = inet_ntoa(clientAddress.sin_addr);
        std::cout << "Source IP Address: " << sourceIP << std::endl;

        // // Retrieve message
        std::string message(buffer, bytesRead);
        std::cout << "Message: " << message << std::endl;
        
        //  save new device 

    }

    // Close the socket
    close(serverSocket);

    return 0;
}


void network_handler(){

    scan("192.168.3.255",GPID_E_PORT, "Show me who you are!");


    if(fuck){
        scan("192.168.3.255",GPID_E_PORT, "Show me who you are!");
    
    }
    std::cout << "hello world" << std::endl;
}

int client_connection_handler(){

// Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }



    // Bind the socket to a specific port
    int port = GPID_E_PORT;  // Change this to the desired port number
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Failed to bind socket to port " << port << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "Server is listening for UDP broadcast on port " << port << std::endl;

    // Receive incoming packets
    char buffer[BUFFER_SIZE];
    sockaddr_in clientAddress{};
    socklen_t clientAddressLength = sizeof(clientAddress);

    while (true) {
        // Receive packet
        int bytesRead = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&clientAddress, &clientAddressLength);
        if (bytesRead < 0) {
            std::cerr << "Failed to receive packet" << std::endl;
            close(serverSocket);
            return 1;
        }

        
        // Retrieve source IP address
        std::string sourceIP = inet_ntoa(clientAddress.sin_addr);
        std::cout << "Source IP Address: " << sourceIP << std::endl;

        // // Retrieve message
        std::string message(buffer, bytesRead);
        std::cout << "Message: " << message << std::endl;
        
        //  save new device 

    }

    // Close the socket
    close(serverSocket);

    return 0;

}