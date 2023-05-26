#pragma once
#include "mqtt_client.h"
#include <iostream>
#include "db.h"
#include <vector>
#define GPID_E_PORT 42069
#define BUFFER_SIZE 1024

bool scan(const std::string& ipAddress, const std::string& message);
bool client_tcp_handler();
int retrieve_information(const std::string& ipAddress, const std::string& message);
int wait_for_clients_broadcast();
int send_tcp_data(const std::string &server_ip);
void network_handler();
bool send_data(int socket, const std::string& data);
int handle_client_creation(int client_socket, std::string ip_address);
std::vector<std::string> split(const std::string &s, char seperator);
