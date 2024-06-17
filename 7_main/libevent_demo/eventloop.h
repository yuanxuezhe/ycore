#include <event2/event.h>
#include <functional>
#include <iostream>
#include "eventbase.h"

class Event {
public:
    Event(EventBase& eventBase, std::function<void(int, short)> callback)
        : base(eventBase.getBase()), callback(callback) {
        
        event = event_new(base, -1, 0, &Event::eventCallback, this);
        if (!event) {
            throw std::runtime_error("Could not create new event!");
        }
    }

    Event(EventBase& eventBase, int fd, short flags, std::function<void(int, short)> callback)
        : base(eventBase.getBase()), callback(callback) {
        
        event = event_new(base, fd, flags, &Event::eventCallback, this);
        if (!event) {
            throw std::runtime_error("Could not create new event!");
        }
    }

    ~Event() {
        if (event) {
            event_free(event);
        }
    }

    void add(struct timeval* timeout = nullptr) {
        if (event_add(event, timeout) < 0) {
            throw std::runtime_error("Could not add event!");
        }
    }

    void del() {
        if (event_del(event) < 0) {
            throw std::runtime_error("Could not delete event!");
        }
    }

private:
    static void eventCallback(evutil_socket_t fd, short what, void* arg) {
        Event* ev = static_cast<Event*>(arg);
        ev->callback(fd, what);
        ev->add(); // 重新添加事件以便下次触发
    }

    struct event_base* base;
    struct event* event;
    std::function<void(int, short)> callback;
};
