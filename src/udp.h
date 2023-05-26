#pragma once

#include <iostream>
#define GPID_E_PORT 42069
#define BUFFER_SIZE 1024

bool sendUdpMessage(const std::string& ipAddress, int port, const std::string& message);
int awaitResponse();