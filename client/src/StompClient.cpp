#include <iostream>
#include <string>
#include <cstdlib>
#include "ConnectionHandler.h"
#include "StompProtocol.h"

class StompClient {
private:
	bool LoggedIn = false;
	ConnectionHandler *connectionHandler;

	void processResponse(const std::string& response) {
		if (response.find("CONNECTED") != std::string::npos) {
			std::cout << "Login successful" << std::endl;
			isLoggedIn = true;
		} else if (response.find("ERROR") != std::string::npos) {
			if (response.find("Wrong password") != std::string::npos) {
				std::cout << "Wrong password" << std::endl;
			}
		}
	}

public:

	StompClient(std::string host, short port) {
		connectionHandler = new ConnectionHandler(host, port);
	}
	~StompClient() {
		delete connectionHandler;
	}

	void subscribe(const std::string& channelName) {
		if (!isLoggedIn) {
			std::cout << "You need to logged in order to subscribe" << std::endl;
			return;
		}
		std::string frame = StompProtocol::subscribeFrame(channelName,int id);

		if (!connectionHandler->sendBytes(frame.c_str(), frame.length())) {
			std::cerr << "Failed to send SUBSCRIBE frame" << std::endl;
			return;
		}
		std::cout << "Subscribed to " << channelName << std::endl;
	}

        void unsubscribe(const std::string& channelName) {
		if (!isLoggedIn) {
			std::cout << "You need to logged in order to usubscribe" << std::endl;
			return;
		}
		std::string frame = StompProtocol::unsubscribeFrame(channelName);
		if (!connectionHandler->sendBytes(frame.c_str(), frame.length())) {
			std::cerr << "Failed to send UNSUBSCRIBE frame" << std::endl;
			return;
		}
		std::cout << "Unsubscribed from " << channelName << std::endl;
	}

	void disconnect() {
		if (!isLoggedIn) {
			std::cout << "You must be logged in to disconnect" << std::endl;
			return;
		}
		std::string frame = StompProtocol::createDisconnectFrame();
		if (!connectionHandler->sendBytes(frame.c_str(), frame.length())) {
			std::cerr << "Failed to send DISCONNECT frame" << std::endl;
			return;
		}

		std::cout << "Disconnected" << std::endl;
		isLoggedIn = false;
	}
};


void disconnect() {
	if (!isLoggedIn) {
		std::cout << "You must be logged in to disconnect" << std::endl;
		return;
	}
	std::string frame = StompProtocol::disconnectFrame();
	if (!connectionHandler->sendBytes(frame.c_str(), frame.length())) {
		std::cerr << "Failed to send DISCONNECT frame" << std::endl;
		return;
	}
	std::cout << "Disconnected" << std::endl;
	isLoggedIn = false;
    }
 }



int main(int argc, char *argv[]) {
	// TODO: implement the STOMP client







}