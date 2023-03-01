#ifndef I_OBSERVER_H
#define I_OBSERVER_H

#include <cstdint>

class IObserver {
public:
    virtual void handle(int16_t code) = 0;
    virtual ~IObserver() = default;
};

#endif
