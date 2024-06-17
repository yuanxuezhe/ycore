#include <iostream>
#include <unistd.h>
#include <signal.h>
#include "eventloop.h"
// // 信号处理器
// void signalHandler(int signal, short events) {
//     std::cout << "Caught signal " << signal << std::endl;
//     //event_base_loopbreak(global_base); // 停止事件循环
// }

// int main() {
//     try {
//         EventBase base;

//                 // 创建时间事件，每隔2秒触发一次
//         Event timerEvent(base, signalHandler);
//         struct timeval tv = {2, 0}; // 2秒
//         timerEvent.add(&tv);

//         // 创建信号事件
//         //Event signalEvent(base, SIGINT, EV_SIGNAL | EV_PERSIST, signalHandler);
//         //signalEvent.add();

//         std::cout << "Event loop started, press Ctrl+C to exit..." << std::endl;
//         base.dispatch();
//         std::cout << "Event loop exited." << std::endl;
//     } catch (const std::exception& e) {
//         std::cerr << "Exception: " << e.what() << std::endl;
//     }

//     return 0;
// }

// 定时器处理器
void timerHandler(int fd, short events) {
    std::cout << "Timer event triggered!" << std::endl;
}

int main() {
    try {
        EventBase base;

        // 创建时间事件，每隔2秒触发一次
        Event timerEvent(base, timerHandler);
        struct timeval tv = {2, 0}; // 2秒
        timerEvent.add(&tv);

        std::cout << "Event loop started, waiting for timer events..." << std::endl;
        base.dispatch();
        std::cout << "Event loop exited." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
