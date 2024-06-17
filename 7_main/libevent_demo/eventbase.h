#include <event2/event.h>
#include <stdexcept>
#include <iostream>

class EventBase {
public:
    EventBase() {
        base = event_base_new();
        if (!base) {
            throw std::runtime_error("Could not initialize libevent!");
        }
    }

    ~EventBase() {
        event_base_free(base);
    }

    void dispatch() {
        event_base_dispatch(base);
    }

    struct event_base* getBase() const {
        return base;
    }

private:
    struct event_base* base;
};
