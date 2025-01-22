#pragma once
#include <string>
#include <thread>
#include <atomic>
#include "ConnectionHandler.h"
#include "KeyboardHandler.h"
#include "StompProtocol.h"

#include <string>
#include <thread>
#include <atomic>


class StompClient {
public:
    // Constructor
    StompClient();

    // Destructor
    ~StompClient();

    // Starts the client runtime
    void start();

    // Stops the client runtime
    void stop();

    // Login to the server
    bool login(const std::string& host, short port, const std::string& username, const std::string& password);

    // Command methods
    void subscribe(const std::string& channelName, int id) const;
    void unsubscribe(int id, const std::string& channelName) const;
    void disconnect(int id);
    void send(const std::string& destination, const std::string& data) const;

private:
    void processLogin(const std::string& response); // Process server responses
    void handleConnection(); // Thread for handling server communication
    void processResponse();

    ConnectionHandler* connectionHandler; // Pointer to handle communication
    std::thread connectionThread;         // Connection thread
    std::atomic<bool> running;            // Controls thread execution
    bool LoggedIn;
};
