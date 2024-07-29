cmake_minimum_required(VERSION 3.10)
project(TCPServerCLI)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/clientManager.cpp
    src/ClientResponse.cpp
    src/ServerQueries.cpp
    src/tcpNetworkManager.cpp
)

# Header files
set(HEADERS
    include/clientManager.h
    include/ClientResponse.h
    include/ServerQueries.h
    include/tcpNetworkManager.h
)

# Group source files
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Source Files" FILES ${SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include PREFIX "Header Files" FILES ${HEADERS})

# Enable Unicode
add_definitions(-D_UNICODE -DUNICODE)

# Set runtime library to /MT for release builds
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")

# Executable
add_executable(TCPServerCLI ${SOURCES} ${HEADERS})

# Link with Ws2_32.lib
target_link_libraries(TCPServerCLI Ws2_32)

# Handle 32-bit and 64-bit builds
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "Configuring for 64-bit build")
else()
    message(STATUS "Configuring for 32-bit build")
endif()
