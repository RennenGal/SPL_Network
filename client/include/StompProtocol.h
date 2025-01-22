#pragma once
#include <string>
using namespace std;
// TODO: implement the STOMP protocol
class StompProtocol{
    StompProtocol() = default;
public:
    static string connectFrame(const string& username, const string& password);
    static string sendFrame(const string& destination, const string& message);
    static string subscribeFrame(const string& destination, int id);
    static string unsubscribeFrame(int id);
    static string disconnectFrame(int receiptId);

};
