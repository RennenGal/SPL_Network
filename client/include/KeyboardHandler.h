#pragma once
#include "StompClient.h"
class KeyboardHandler {
    // Thread function for handling input
    void run();
    StompClient& client;       // Reference to the client for communication
    std::thread inputThread;   // Thread for handling keyboard input
    std::atomic<bool> running; // Flag to control the thread loop

    public:
    // Constructor
    KeyboardHandler(StompClient& client);

    // Destructor
    ~KeyboardHandler();

    // Starts the thread to handle keyboard input
    void start();

    // Stops the thread safely
    void stop();


};