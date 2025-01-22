#include "StompClient.h"
#include <iostream>
#include <string>
#include "ConnectionHandler.h"
#include "StompProtocol.h"
using namespace std;

// Constructor
StompClient::StompClient()
    : connectionHandler(nullptr), running(false), LoggedIn(false) {}

// Destructor
StompClient::~StompClient() {
    stop();
	delete connectionHandler;
}

// Login to the server
bool StompClient::login(const string& host, const short port, const string& username, const string& password) {
    if (LoggedIn) {
        cout << "The client is already logged in, log out before trying again" << endl;
        return false;
    }

    connectionHandler = new ConnectionHandler(host, port);
    if (!connectionHandler->connect()) {
        cerr << "Could not connect to server" << endl;
        delete connectionHandler;
        connectionHandler = nullptr;
        return false;
    }

    const string loginFrame = StompProtocol::connectFrame(username, password);
    if (!connectionHandler->sendBytes(loginFrame.c_str(), static_cast<int>(loginFrame.length()))) {
    	// maybe delete this part
        cerr << "Failed to send login frame" << endl;
        delete connectionHandler;
        connectionHandler = nullptr;
        return false;
    }

    cout << "Login frame sent. Waiting for response..." << endl;
    string response;
    if (connectionHandler->receive(response)) {
        processLogin(response);
        return LoggedIn;
    }

    cerr << "Failed to receive login response" << endl;
    delete connectionHandler;
    connectionHandler = nullptr;
    return false;
}

// Starts the client runtime
void StompClient::start() {
    running.store(true);

    if (!LoggedIn) {
        cerr << "Cannot start client: not logged in!" << endl;
        running.store(false);
        return;
    }

    connectionThread = thread(&StompClient::handleConnection, this);

    if (connectionThread.joinable()) {
        connectionThread.join();
    }
}

// Stops the client runtime
void StompClient::stop() {
    running.store(false);

    if (connectionThread.joinable()) {
        connectionThread.join();
    }

    if (connectionHandler) {
        delete connectionHandler;
        connectionHandler = nullptr;
    }
}

// Handles server communication
void StompClient::handleConnection() {
	while (running.load()) {
		string response;
		if (connectionHandler->receive(response)) {
			processResponse(response);
		}
	}
}

// Process server responses
void StompClient::processLogin(const string& response) {
	if (response.find("CONNECTED") != string::npos) {
		cout << "Login successful" << endl;
		LoggedIn = true;
	} else if (response.find("ERROR") != string::npos) {
		if (response.find("Wrong password") != string::npos) {
			cout << "Wrong password" << endl;
		}
	}
}

void StompClient::subscribe(const string& channelName, const int id) const{
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

void StompClient::unsubscribe(const int id, const string& channelName) const {
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

void StompClient::disconnect(const int id) {
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

void StompClient::send(const string& destination, const string& data) const{
	const string frame = StompProtocol::sendFrame(destination,data);
	if (!connectionHandler->sendBytes(frame.c_str(), static_cast<int>(frame.length()))){
		cerr << "Failed to send frame" << endl;
	}
}

bool StompClient::connect(const string& username, const string& password){
	connectionHandler->connect()
}



int main(int argc, char *argv[]) {
	// TODO: implement the STOMP client
	if (argc != 3) {
		cerr << "Usage: " << argv[0] << " <host> <port>" << endl;
		return 1;
	}

	const string host = argv[1];
	const short port = static_cast<short>(stoi(argv[2]));

	try {
		// Create and run the client
		StompClient client(host, port);
		client.start();
	} catch (const exception& e) {
		cerr << "Error: " << e.what() << endl;
		return 1;
	}

	return 0;
}