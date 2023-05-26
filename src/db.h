#pragma once

#include <postgresql/libpq-fe.h>
#include <string>
#include <vector>

struct device {
	int id;
	std::string ip_address;
	std::string sys_description;
};

struct data_entry {
	int id;
	int timestamp;
	int device_id;
	float consumption;
};

class db {
	db();
	PGconn * connection;
	void save_device(std::string ip_address, std::string sys_description);
	std::vector<device> devices();
	void save_entry(std::string ip_address, float consumption);
};