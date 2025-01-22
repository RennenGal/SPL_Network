#include "KeyboardHandler.h"
#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "StompClient.h"

// Constructor
KeyboardHandler::KeyboardHandler(StompClient& client)
    : client(client), running(false) {}

// Destructor
KeyboardHandler::~KeyboardHandler() {
    stop();
}

// Starts the thread to handle keyboard input
void KeyboardHandler::start() {
    if (running.load()) {
        throw std::runtime_error("KeyboardHandler is already running.");
    }
    running.store(true);
    inputThread = std::thread(&KeyboardHandler::run, this);
}

// Stops the thread safely
void KeyboardHandler::stop() {
    if (running.load()) {
        running.store(false);
        if (inputThread.joinable()) {
            inputThread.join();
        }
    }
}

// Thread function for handling input
void KeyboardHandler::run() {
    std::string input;
    while (running.load()) {
        std::getline(std::cin, input); // Read input from keyboard
        if (!running.load()) break; // Check if the thread is still running

        // Parse and process commands
        std::istringstream iss(input);
        std::string command;
        iss >> command;

        if (command == "login") {
            std::string username, password;
            iss >> username >> password;
            client.login(username, password);
        } else if (command == "subscribe") {
            std::string channel;
            int id;
            iss >> channel >> id;
            client.subscribe(channel, id);
        } else if (command == "unsubscribe") {
            int id;
            iss >> id;
            client.unsubscribe(id);
        } else if (command == "send") {
            std::string destination, message;
            iss >> destination;
            std::getline(iss, message);
            client.send(destination, message);
        } else if (command == "disconnect") {
            client.disconnect();
            stop(); // Stop the keyboard handler after disconnecting
        } else {
            std::cerr << "Unknown command: " << command << std::endl;
        }
    }
}