cmake_minimum_required(VERSION 3.29)
project(Assignment_3_JetBrains)

set(CMAKE_CXX_STANDARD 14)

include_directories(client/include)

add_executable(Assignment_3_JetBrains
    client/bin/ConnectionHandler.o
    client/bin/EchoClient
    client/bin/echoClient.o
    client/bin/StompESClient
    client/data/events1.json
    client/data/events1_partial.json
    client/include/ConnectionHandler.h
    client/include/event.h
    client/include/json.hpp
    client/include/StompProtocol.h
    client/src/ConnectionHandler.cpp
    client/src/echoClient.cpp
    client/src/event.cpp
    client/src/StompClient.cpp)
