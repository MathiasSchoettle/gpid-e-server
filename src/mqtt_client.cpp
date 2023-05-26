// async_subscribe.cpp
//
// This is a Paho MQTT C++ client, sample application.
//
// This application is an MQTT subscriber using the C++ asynchronous client
// interface, employing callbacks to receive messages and status updates.
//
// The sample demonstrates:
//  - Connecting to an MQTT server/broker.
//  - Subscribing to a topic
//  - Receiving messages through the callback API
//  - Receiving network disconnect updates and attempting manual reconnects.
//  - Using a "clean session" and manually re-subscribing to topics on
//    reconnect.
//

/*******************************************************************************
 * Copyright (c) 2013-2020 Frank Pagliughi <fpagliughi@mindspring.com>
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Frank Pagliughi - initial implementation and documentation
 *******************************************************************************/
#include "mqtt_client.h"

/////////////////////////////////////////////////////////////////////////////

// Callbacks for the success or failures of requested actions.
// This could be used to initiate further action, but here we just log the
// results to the console.

void action_listener::on_failure(const mqtt::token& tok) {
		std::cout << name << " failure";
		if (tok.get_message_id() != 0)
			std::cout << " for token: [" << tok.get_message_id() << "]" << std::endl;
		std::cout << std::endl;
	}

void action_listener::on_success(const mqtt::token& tok) {
		std::cout << name << " success";
		if (tok.get_message_id() != 0)
			std::cout << " for token: [" << tok.get_message_id() << "]" << std::endl;
		auto top = tok.get_topics();
		if (top && !top->empty())
			std::cout << "\ttoken topic: '" << (*top)[0] << "', ..." << std::endl;
		std::cout << std::endl;
	}

/////////////////////////////////////////////////////////////////////////////

/**
 * Local callback & listener class for use with the client connection.
 * This is primarily intended to receive messages, but it will also monitor
 * the connection to the broker. If the connection is lost, it will attempt
 * to restore the connection and re-subscribe to the topic.
 */
	void callback::reconnect() {
		std::this_thread::sleep_for(std::chrono::milliseconds(2500));
		try {
			cli.connect(connOpts, nullptr, *this);
		}
		catch (const mqtt::exception& exc) {
			std::cerr << "Error: " << exc.what() << std::endl;
			exit(1);
		}
	}

	// Re-connection failure
	void callback::on_failure(const mqtt::token& tok) {
		std::cout << "Connection attempt failed" << std::endl;
		if (++nretry > N_RETRY_ATTEMPTS)
			exit(1);
		reconnect();
	}

	// (Re)connection success
	// Either this or connected() can be used for callbacks.
	void callback::on_success(const mqtt::token& tok) {}

	// (Re)connection success
	void callback::connected(const std::string& cause) {
		std::cout << "\nConnection success" << std::endl;
		std::cout << "\nSubscribing to topic '" << TOPIC << "'\n"
			<< "\tfor client " << CLIENT_ID
			<< " using QoS" << QOS << "\n"
			<< "\nPress Q<Enter> to quit\n" << std::endl;

		cli.subscribe(TOPIC, QOS, nullptr, subListener);
	}

	// Callback for when the connection is lost.
	// This will initiate the attempt to manually reconnect.
	void callback::connection_lost(const std::string& cause) {
		std::cout << "\nConnection lost" << std::endl;
		if (!cause.empty())
			std::cout << "\tcause: " << cause << std::endl;

		std::cout << "Reconnecting..." << std::endl;
		nretry = 0;
		reconnect();
	}

	// Callback for when a message arrives.
	void callback::message_arrived(mqtt::const_message_ptr msg) {
		std::cout << "Message arrived" << std::endl;
		std::cout << "\ttopic: '" << msg->get_topic() << "'" << std::endl;
		std::cout << "\tpayload: '" << msg->to_string() << "'\n" << std::endl;
	}

	void callback::delivery_complete(mqtt::delivery_token_ptr token) {}

int mqtt_start_scan() {
	// A subscriber often wants the server to remember its messages when its
	// disconnected. In that case, it needs a unique ClientID and a
	// non-clean session.

	mqtt::async_client cli(SERVER_ADDRESS, CLIENT_ID);

	mqtt::connect_options connOpts;
	connOpts.set_clean_session(false);

	// Install the callback(s) before connecting.
	callback cb(cli, connOpts);
	cli.set_callback(cb);

	// Start the connection.
	// When completed, the callback will subscribe to topic.

	try {
		std::cout << "Connecting to the MQTT server..." << std::flush;
		cli.connect(connOpts, nullptr, cb);
	}
	catch (const mqtt::exception& exc) {
		std::cerr << "\nERROR: Unable to connect to MQTT server: '"
			<< SERVER_ADDRESS << "'" << exc << std::endl;
		return 1;
	}

	// Just block till user tells us to quit.

	while (std::tolower(std::cin.get()) != 'q')
		;

	// Disconnect

	try {
		std::cout << "\nDisconnecting from the MQTT server..." << std::flush;
		cli.disconnect()->wait();
		std::cout << "OK" << std::endl;
	}
	catch (const mqtt::exception& exc) {
		std::cerr << exc << std::endl;
		return 1;
	}

 	return 0;

}


/////////////////////////////////////////////////////////////////////////////

//int main(int argc, char* argv[])
//{
//	// A subscriber often wants the server to remember its messages when its
//	// disconnected. In that case, it needs a unique ClientID and a
//	// non-clean session.
//
//	mqtt::async_client cli(SERVER_ADDRESS, CLIENT_ID);
//
//	mqtt::connect_options connOpts;
//	connOpts.set_clean_session(false);
//
//	// Install the callback(s) before connecting.
//	callback cb(cli, connOpts);
//	cli.set_callback(cb);
//
//	// Start the connection.
//	// When completed, the callback will subscribe to topic.
//
//	try {
//		std::cout << "Connecting to the MQTT server..." << std::flush;
//		cli.connect(connOpts, nullptr, cb);
//	}
//	catch (const mqtt::exception& exc) {
//		std::cerr << "\nERROR: Unable to connect to MQTT server: '"
//			<< SERVER_ADDRESS << "'" << exc << std::endl;
//		return 1;
//	}
//
//	// Just block till user tells us to quit.
//
//	while (std::tolower(std::cin.get()) != 'q')
//		;
//
//	// Disconnect
//
//	try {
//		std::cout << "\nDisconnecting from the MQTT server..." << std::flush;
//		cli.disconnect()->wait();
//		std::cout << "OK" << std::endl;
//	}
//	catch (const mqtt::exception& exc) {
//		std::cerr << exc << std::endl;
//		return 1;
//	}
//
// 	return 0;
//}
