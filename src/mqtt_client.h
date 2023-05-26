
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cctype>
#include <thread>
#include <chrono>
#include <MQTTAsync.h>
#include <mqtt/async_client.h>
#include <MQTTProperties.h>

static const std::string SERVER_ADDRESS("tcp://localhost:1883");
static const std::string CLIENT_ID("paho_cpp_async_subcribe");
static const std::string TOPIC("start_scan");

static const int	QOS = 1;
static const int	N_RETRY_ATTEMPTS = 5;



class action_listener : public virtual mqtt::iaction_listener
{
	std::string name;

	void on_failure(const mqtt::token& tok) override;
	void on_success(const mqtt::token& tok) override;

public:
	action_listener(const std::string& name) : name(name) {}
};


class callback : public virtual mqtt::callback,
					public virtual mqtt::iaction_listener

{
	// Counter for the number of connection retries
	int nretry;
	// The MQTT client
	mqtt::async_client& cli;
	// Options to use if we need to reconnect
	mqtt::connect_options& connOpts;
	// An action listener to display the result of actions.
	action_listener subListener;

	// This deomonstrates manually reconnecting to the broker by calling
	// connect() again. This is a possibility for an application that keeps
	// a copy of it's original connect_options, or if the app wants to
	// reconnect with different options.
	// Another way this can be done manually, if using the same options, is
	// to just call the async_client::reconnect() method.
	void reconnect();
	// Re-connection failure
	void on_failure(const mqtt::token& tok) override;
	// (Re)connection success
	// Either this or connected() can be used for callbacks.
	void on_success(const mqtt::token& tok) override;

	// (Re)connection success
	void connected(const std::string& cause) override;
	// Callback for when the connection is lost.
	// This will initiate the attempt to manually reconnect.
	void connection_lost(const std::string& cause) override;
	// Callback for when a message arrives.
	void message_arrived(mqtt::const_message_ptr msg) override;
	void delivery_complete(mqtt::delivery_token_ptr token) override;

public:
	callback(mqtt::async_client& cli, mqtt::connect_options& connOpts)
				: nretry(0), cli(cli), connOpts(connOpts), subListener("Subscription") {};
};
