#pragma once
#include <string>
#include <map>
#include "../include/ConnectionHandler.h"


// TODO: implement the STOMP protocol
class StompProtocol{
private:
  ConnectionHandler connection;

public:
    StompProtocol(ConnectionHandler& connectionHandler);
    virtual ~StompProtocol();


    std::string connectFrame(const std::string& username, const std::string& password);
    std::string sendFrame(const std::string& destination, const std::string& message);
    std::string subscribeFrame(const std::string& destination, int id);
    std::string unsubscribeFrame(int id);
    std::string disconnectFrame(int receiptId);







};
