#include "networkHandler.h"
#include "mqtt_client.h"
#include <iostream>
#include <ostream>
#include <thread>

int main() {
	std::string ipAddress = "192.168.3.255";  // IP address of the destination
	int port = GPID_E_PORT;                  // Port number of the destination
	std::string message = "Broadcast!";  // Message to send

	// Call the function to send the UDP message
	// for(int i = 0; i < 100; i++)
	// 	sendUdpMessage(ipAddress, port, message);

	//awaitResponse();
  //mqtt_start_scan();
  std::thread t_network_listener(network_handler);
  
  std::thread t_scan_waiter(mqtt_start_scan);
  
  std::cout << "Der Bré wartet" << std::endl;

  t_scan_waiter.join();

	return 0;
}
