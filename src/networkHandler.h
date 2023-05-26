#pragma once
#include "mqtt_client.h"
#include <iostream>
#define GPID_E_PORT 42069
#define BUFFER_SIZE 1024

bool scan(const std::string& ipAddress, int port, const std::string& message);
int wait_for_clients();

void network_handler();
client_connection_handler();