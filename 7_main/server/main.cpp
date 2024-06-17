#include <iostream>
#include "tcpserver.h"

void onNewConnection(std::shared_ptr<TCPConnection> conn) {
    std::cout << "New connection established!" << std::endl;

    conn->setReadCallback([](const std::string& data) {
        std::cout << "Received data: " << data << std::endl;
    });

    conn->setEventCallback([](short events) {
        if (events & BEV_EVENT_ERROR) {
            std::cerr << "Error on connection!" << std::endl;
        }
        if (events & BEV_EVENT_EOF) {
            std::cout << "Connection closed." << std::endl;
        }
    });
}

int main() {
    try {
        EventCenter server(12345, onNewConnection);
        std::cout << "Server started, waiting for connections..." << std::endl;
        server.start();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
