#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ether.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "networkHandler.h"

bool scan(const std::string &ipAddress, const std::string &message)
{
    // Create a UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    int broadcast = 1;
    if (sockfd < 0)
    {
        std::cerr << "Failed to create socket." << std::endl;
        return false;
    }

    // Set up the destination address
    sockaddr_in destAddr{};
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(GPID_E_PORT);
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST,
                   &broadcast, sizeof broadcast) < 0)
        printf("setsockopt");

    if (inet_pton(AF_INET, ipAddress.c_str(), &(destAddr.sin_addr)) <= 0)
    {
        std::cerr << "Invalid IP address." << std::endl;
        close(sockfd);
        return false;
    }

    // Send the UDP message
    ssize_t sentBytes = sendto(sockfd, message.c_str(), message.size(), 0,
                               reinterpret_cast<struct sockaddr *>(&destAddr), sizeof(destAddr));
    if (sentBytes < 0)
    {
        std::cerr << "Failed to send UDP message." << std::endl;
        close(sockfd);
        return false;
    }

    std::cout << "UDP message sent successfully." << std::endl;

    // Close the socket
    close(sockfd);
    return true;
}

int wait_for_clients_broadcast()
{
    // Create a socket
    int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket < 0)
    {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    // Enable broadcast
    int broadcastEnable = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable)) < 0)
    {
        std::cerr << "Failed to enable broadcast." << std::endl;
        close(serverSocket);
        return 1;
    }

    // Bind the socket to a specific port
    int port = GPID_E_PORT; // Change this to the desired port number
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        std::cerr << "Failed to bind socket to port " << port << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "Server is listening for UDP broadcast on port " << port << std::endl;

    // Receive incoming packets
    char buffer[BUFFER_SIZE];
    sockaddr_in clientAddress{};
    socklen_t clientAddressLength = sizeof(clientAddress);

    while (true)
    {
        // Receive packet
        int bytesRead = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&clientAddress, &clientAddressLength);
        if (bytesRead < 0)
        {
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
        std::string sysDesc = split(message, ':')[1];
        std::cout << "Found new device with IP: " << sourceIP << " and a SysDesc of: " << sysDesc;
        database->save_device(sourceIP, sysDesc);   
        
    }

    // Close the socket
    close(serverSocket);

    return 0;
}

void network_handler()
{
    for (int i = 0; i < 5; i++)
    {
        scan("192.168.3.255", "Show me who you are!");
        // send_tcp_data("192.168.3.1");
        sleep(10);
    }
    // send_tcp_data("192.168.3.1");
    if (fuck)
    {
        scan("192.168.3.255", "Show me who you are!");
    }
}

int retrieve_information(const std::string &ipAddress, const std::string &message)
{

    // Create a TCP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    int broadcast = 1;
    if (sockfd < 0)
    {
        std::cerr << "Failed to create socket." << std::endl;
        return false;
    }

    // Set up the destination address
    sockaddr_in destAddr{};
    destAddr.sin_family = AF_INET;
    destAddr.sin_port = htons(GPID_E_PORT);

    if (inet_pton(AF_INET, ipAddress.c_str(), &(destAddr.sin_addr)) <= 0)
    {
        std::cerr << "Invalid IP address." << std::endl;
        close(sockfd);
        return false;
    }

    // Send the UDP message
    ssize_t sentBytes = sendto(sockfd, message.c_str(), message.size(), 0,
                               reinterpret_cast<struct sockaddr *>(&destAddr), sizeof(destAddr));
    if (sentBytes < 0)
    {
        std::cerr << "Failed to send TCP message." << std::endl;
        close(sockfd);
        return false;
    }

    std::cout << "UDP message sent successfully." << std::endl;

    // Close the socket
    close(sockfd);
    return true;
}

bool client_tcp_handler()
{
    // Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    // Bind the socket to a specific port
    int port = GPID_E_PORT; // Change this to the desired port number
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        std::cerr << "Failed to bind socket to port " << port << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "Server is listening for TCP on port " << port << std::endl;

    // Receive incoming packets
    char buffer[BUFFER_SIZE];
    sockaddr_in clientAddress{};
    socklen_t clientAddressLength = sizeof(clientAddress);
    if (listen(serverSocket, 100) < 0)
    {
        std::cerr << "Failed while listening to socket port";
        close(serverSocket);
        return 1;
    }
    int client_socket = accept(serverSocket, reinterpret_cast<struct sockaddr *>(&clientAddress), &clientAddressLength);
    if (client_socket == -1)
    {
        std::cerr << "Error accepting client connection" << std::endl;
        close(serverSocket);
        return -1;
    }

    // Handle the client
    handle_client_creation(client_socket);

    // Close the client socket
    close(client_socket);
    // Close the socket
    close(serverSocket);

    return 0;
}

std::string read_data(int client_socket)
{

    const int buffer_size = 1024;
    char buffer[buffer_size];
    ssize_t bytes_read = read(client_socket, buffer, buffer_size - 1);
    if (bytes_read == -1)
    {
        std::cerr << "Error reading from client" << std::endl;
        return "error";
    }

    buffer[bytes_read] = '\0';
    std::string read_data = std::string(buffer);

    // Process the received message
    std::cout << "Received: " << read_data << std::endl;

    return read_data;
}

int handle_client_creation(int client_socket)
{
    const int buffer_size = 1024;
    char buffer[buffer_size];

    std::string received_data = read_data(client_socket);

    // Process the received message

    // TODO Save Client
}

int connect_to_server(const std::string &server_ip)
{
    // Create a socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }

    // Set up the server address
    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(GPID_E_PORT);
    if (inet_pton(AF_INET, server_ip.c_str(), &(server_address.sin_addr)) <= 0)
    {
        std::cerr << "Invalid server address" << std::endl;
        close(client_socket);
        return -1;
    }

    // Connect to the server
    if (connect(client_socket, reinterpret_cast<struct sockaddr *>(&server_address), sizeof(server_address)) == -1)
    {
        std::cerr << "Error connecting to the server" << std::endl;
        close(client_socket);
        return -1;
    }

    std::cout << "Connected to the server" << std::endl;

    return client_socket;
}

bool send_data(int socket, const std::string &data)
{
    ssize_t bytes_written = write(socket, data.c_str(), data.length());
    if (bytes_written == -1)
    {
        std::cerr << "Error writing to server" << std::endl;
        return false;
    }

    std::cout << "Data sent: " << data << std::endl;
    // const int buffer_size = 1024;
    // char buffer[buffer_size];
    // ssize_t bytes_read = read(socket, buffer, buffer_size - 1);
    // std::cout << "Data read: " << std::string(buffer) << std::endl;
    return true;
}

int send_tcp_data(const std::string &server_ip)
{

    std::cout << "Lutsch den Arsch!" << std::endl;
    // Connect to the server
    int client_socket = connect_to_server(server_ip);
    if (client_socket == -1)
    {
        std::cerr << "Failed to connect to the server" << std::endl;
        return 1;
    }

    // Send data to the server
    std::string message = "COMMAND:SEND_DATATYPE\r";
    if (!send_data(client_socket, message))
    {
        std::cerr << "Failed to send data to the consumer" << std::endl;
        close(client_socket);
        return 1;
    }
    std::string received_data = read_data(client_socket);

    // Save Dataenty

    // Close the client socket
    close(client_socket);

    return 0;
}

std::vector<std::string> split(const std::string &s, char seperator)
{
    std::vector<std::string> output;

    std::string::size_type prev_pos = 0, pos = 0;

    while ((pos = s.find(seperator, pos)) != std::string::npos)
    {
        std::string substring(s.substr(prev_pos, pos - prev_pos));

        output.push_back(substring);

        prev_pos = ++pos;
    }

    output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word

    return output;
}