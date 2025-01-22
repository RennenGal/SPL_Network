#include "StompProtocol.h"
using namespace std;
const string acceptVERSION = "1.2";
const string HOST = "stomp.cs.bgu.ac.il";
string StompProtocol::connectFrame(const string& username, const string& password) {
    string frame = "CONNECT\n";
    frame += "accept-version:" + acceptVERSION + "\n";
    frame += "host:" + HOST + "\n";
    frame += "login:" + username + "\n";
    frame += "passcode:" + password + "\n";
    frame += "\n";
    frame += '\0';
    return frame;
}


string StompProtocol::sendFrame(const string& destination, const string& message) {
    string frame = "SEND\n";
    frame += "destination:" + destination + "\n";
    frame += "\n";
    frame += message;
    frame += "\0";
    return frame;
}

string StompProtocol::subscribeFrame(const string& destination, int id) {
    string frame = "SUBSCRIBE\n";
    frame += "destination:" + destination + "\n";
    frame += "id:" + to_string(id) + "\n";
    frame += "\n";
    frame += '\0';
    return frame;
}


string StompProtocol::unsubscribeFrame(int id) {
    string frame = "UNSUBSCRIBE\n";
    frame += "id:" + to_string(id) + "\n";
    frame += "\n";
    frame += '\0';
    return frame;
}

string StompProtocol::disconnectFrame(int receiptId) {
    string frame = "DISCONNECT\n";
    frame += "receipt:" + to_string(receiptId) + "\n";
    frame += "\n";
    frame += '\0';
    return frame;
}