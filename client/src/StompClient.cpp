#include <iostream>
#include <string>
#include "ConnectionHandler.h"
#include "StompProtocol.h"
using namespace std;

class StompClient {
	bool LoggedIn = false;
	ConnectionHandler *connectionHandler;

	void processResponse(const string& response) {
		if (response.find("CONNECTED") != string::npos) {
			cout << "Login successful" << endl;
			LoggedIn = true;
		} else if (response.find("ERROR") != string::npos) {
			if (response.find("Wrong password") != string::npos) {
				cout << "Wrong password" << endl;
			}
		}
	}

public:

	StompClient(const string& host, const short port) {
		connectionHandler = new ConnectionHandler(host, port);
	}
	~StompClient() {
		delete connectionHandler;
	}

	void subscribe(const string& channelName, const int id) const
	{
		if (!LoggedIn) {
			cout << "You need to logged in order to subscribe" << endl;
			return;
		}
		const string frame = StompProtocol::subscribeFrame(channelName,id);

		if (!connectionHandler->sendBytes(frame.c_str(), static_cast<int>(frame.length()))) {
			cerr << "Failed to send SUBSCRIBE frame" << endl;
			return;
		}
		cout << "Subscribed to " << channelName << endl;
	}

    void unsubscribe(const int id, const string& channelName) const {
		if (!LoggedIn) {
			cout << "You need to logged in order to usubscribe" << endl;
			return;
		}
		const string frame = StompProtocol::unsubscribeFrame(id);
		if (!connectionHandler->sendBytes(frame.c_str(), static_cast<int>(frame.length()))) {
			cerr << "Failed to send UNSUBSCRIBE frame" << endl;
			return;
		}
		cout << "Unsubscribed from " << channelName << endl;
	}

	void disconnect(const int id) {
		if (!LoggedIn) {
			cout << "You must be logged in to disconnect" << endl;
			return;
		}
		const string frame = StompProtocol::disconnectFrame(id);
		if (!connectionHandler->sendBytes(frame.c_str(), static_cast<int>(frame.length()))) {
			cerr << "Failed to send DISCONNECT frame" << endl;
			return;
		}
		cout << "Disconnected" << endl;
		LoggedIn = false;
	}

	bool sendFrame(const string& destination, const string& frame) const
	{
		
		return false;
	}
};




int main(int argc, char *argv[]) {
	// TODO: implement the STOMP client

}