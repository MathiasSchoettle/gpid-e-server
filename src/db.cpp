#include <iostream>
#include <postgresql/libpq-fe.h>

using namespace std;

void test_connection() {

	auto conn = PQconnectdb("dbname=postgres host=localhost user=postgres password=gpid");

	if (PQstatus(conn) == CONNECTION_BAD) {
		puts("We were unable to connect to the database");
		exit(0);
	}
}