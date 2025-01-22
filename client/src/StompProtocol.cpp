
#include "StompProtocol.h"



const std::string StompProtocol::acceptVERSION = "1.2";
const std::string StompProtocol::HOST = "stomp.cs.bgu.ac.il";

std::string StompProtocol::connectFrame(const std::string& username, const std::string& password) {

    std::string frame = "CONNECT\n";
    frame += "accept-version:" + acceptVERSION + "\n";
    frame += "host:" + HOST + "\n";
    frame += "login:" + username + "\n";
    frame += "passcode:" + password + "\n";
    frame += "\n";
    frame += '\0';
    return frame;
}


std::string StompProtocol::sendFrame(const std::string& destination, const std::string& message) {
    std::string frame = "SEND\n";
    frame += "destination:" + destination + "\n";
    frame += "\n";
    frame += message;
    frame += "\0";
    return frame;
}

std::string StompProtocol::subscribeFrame(const std::string& destination, int id) {
    std::string frame = "SUBSCRIBE\n";
    frame += "destination:" + destination + "\n";
    frame += "id:" + std::to_string(id) + "\n";
    frame += "\n";
    frame += '\0';
    return frame;
}


std::string StompProtocol::unsubscribeFrame(int id) {
    std::string frame = "UNSUBSCRIBE\n";
    frame += "id:" + std::to_string(id) + "\n";
    frame += "\n";
    frame += '\0';
    return frame;
}

std::string StompProtocol::disconnectFrame(int receiptId) {
    std::string frame = "DISCONNECT\n";
    frame += "receipt:" + std::to_string(receiptId) + "\n";
    frame += "\n";
    frame += '\0';
    return frame;
}