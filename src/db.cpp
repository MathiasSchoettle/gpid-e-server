#include <iostream>
#include "db.h"

using namespace std;

void test_connection() {

	auto conn = PQconnectdb("dbname=postgres host=localhost user=postgres password=gpid");

	if (PQstatus(conn) == CONNECTION_BAD) {
		puts("We were unable to connect to the database");
		exit(0);
	}
}

db::db() {

}

void db::save_device(std::string ip_address, std::string sys_description) {

}

std::vector<device> db::devices() {

}

void db::save_entry(std::string ip_address, float consumption) {
	
}
