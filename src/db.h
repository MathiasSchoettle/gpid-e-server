#pragma once

#include <postgresql/libpq-fe.h>
#include <string>
#include <vector>

struct device {
	int id;
	std::string ip_address;
	std::string sys_description;
	device(int id, std::string ip_address, std::string sys_description) 
	: id(id), ip_address(ip_address), sys_description(sys_description) {}
};

struct data_entry {
	int id;
	int timestamp;
	int device_id;
	float consumption;
};

class db {
	PGconn *connection;
public:
	db();
	~db();
	bool save_device(std::string &ip_address, std::string &sys_description);
	std::vector<device> devices();
	bool save_entry(const std::string &ip_address, float consumption);
};

extern db *database;
