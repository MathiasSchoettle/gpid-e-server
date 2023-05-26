#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

#include "db.h"

db *database;

db::db() {
	connection = PQconnectdb("dbname=postgres host=localhost user=postgres password=gpid");

	if (PQstatus(connection) == CONNECTION_BAD) {
		std::cerr << "database connection failed, start the fkn postgres you fuck" << std::endl;
		exit(0);
	}
}

db::~db() {
	PQfinish(connection);
}

bool db::save_device(const std::string &ip_address, const std::string &sys_description) {
	const char* insert_query = "INSERT INTO device (deviceip, sys_descr) VALUES ($1, $2)";
	PGresult* prepared_query = PQprepare(connection, "", insert_query, 2, NULL);

	if (PQresultStatus(prepared_query) != PGRES_COMMAND_OK) {
		std::cerr << "Failed to prepare device INSERT statement: " << PQerrorMessage(connection) << std::endl;
		return false;
	}

	const char* values[2];
	values[0] = ip_address.c_str();
	values[1] = sys_description.c_str();
	PGresult* result = PQexecPrepared(connection, "", 2, values, NULL, NULL, 0);

	if (PQresultStatus(result) != PGRES_COMMAND_OK) {
		std::cerr << "Failed to execute device INSERT statement: " << PQerrorMessage(connection) << std::endl;
		PQclear(prepared_query);
		PQclear(result);
		return false;
	}

	PQclear(prepared_query);
	PQclear(result);
	return true;
}

std::vector<device> db::devices() {
	const char* select_query = "SELECT * FROM device";
	PGresult* result = PQexecParams(connection, select_query, 0, NULL, NULL, NULL, NULL, 0);
	std::vector<device> devices;

	if (PQresultStatus(result) != PGRES_TUPLES_OK) {
		std::cerr << "Failed to execute SELECT statement: " << PQerrorMessage(connection) << std::endl;
		PQclear(result);
		return devices;
	}

	for (int i = 0; i < PQntuples(result); ++i) {
		int id = atoi(PQgetvalue(result, i, 0));
		const char* ip_address = PQgetvalue(result, i, 1);
		const char* sys_descr = PQgetvalue(result, i, 2);
		devices.push_back(device(id, ip_address, sys_descr));
	}

	PQclear(result);

	return devices;
}

bool db::save_entry(const std::string &ip_address, int timestamp, float consumption) {
	const char* select_query = "SELECT id FROM device WHERE deviceip = $1";
	const char* param_values[1];
	param_values[0] = ip_address.c_str();

	PGresult* select_result = PQexecParams(connection, select_query, 1, NULL, param_values, NULL, NULL, 0);

	if (PQresultStatus(select_result) != PGRES_TUPLES_OK) {
		std::cerr << "Failed to execute SELECT statement: " << PQerrorMessage(connection) << std::endl;
		PQclear(select_result);
		return false;
	}

	int num_rows = PQntuples(select_result);

	if (num_rows == 0) {
		std::cerr << "Device ID not found for IP address: " << ip_address << std::endl;
		PQclear(select_result);
		return false;
	}

	int device_id = atoi(PQgetvalue(select_result, 0, 0));
	PQclear(select_result);

	auto query = std::string("INSERT INTO data_entry (timestamp, device_id, consumption) VALUES (") + std::to_string(timestamp) + std::string(", ") + std::to_string(device_id) + std::string(",") + std::to_string(consumption) + std::string(")");

	PGresult* insert_result = PQexecParams(connection, query.c_str(), 0, NULL, NULL, NULL, NULL, 0);

	if (PQresultStatus(insert_result) != PGRES_COMMAND_OK) {
		std::cerr << "Failed to execute INSERT statement: " << PQerrorMessage(connection) << std::endl;
		PQclear(insert_result);
		return false;
	}

	PQclear(insert_result);
}
