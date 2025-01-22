cmake_minimum_required(VERSION 3.29)
project(Assignment_3_JetBrains)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 14)

# Include directories
include_directories(client/include)

# Add source files to the executable
add_executable(Assignment_3_JetBrains
        client/src/ConnectionHandler.cpp
        client/src/echoClient.cpp
        client/src/event.cpp
        client/src/StompClient.cpp
        client/include/KeyboardHandler.h
        client/src/KeyboardHandler.cpp
        client/include/StompClient.h
)

# Locate Boost (specify required components)
set(BOOST_ROOT "/usr")
set(BOOST_INCLUDEDIR "/usr/include")
set(BOOST_LIBRARYDIR "/usr/lib")

find_package(Boost REQUIRED COMPONENTS system thread)

if(Boost_FOUND)
    message(STATUS "Boost found!")
    message(STATUS "Boost include directory: ${Boost_INCLUDE_DIR}")
    message(STATUS "Boost libraries: ${Boost_LIBRARIES}")
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(Assignment_3_JetBrains PRIVATE Boost::system Boost::thread)
else()
    message(FATAL_ERROR "Boost not found! Check BOOST_ROOT, BOOST_INCLUDEDIR, and BOOST_LIBRARYDIR.")
endif()
