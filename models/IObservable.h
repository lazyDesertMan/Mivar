#ifndef I_OBSERVABLE_H
#define I_OBSERVABLE_H

#include <memory>
#include <vector>
#include "IObserver.h"

class IObservable {
protected:
    std::vector<std::shared_ptr<IObserver>> m_observers;
    void sendEvent(int16_t code = 0);
public:
    void addObserver(std::shared_ptr<IObserver> observer);
    void removeObserver(const std::shared_ptr<IObserver>& observer);
    ~IObservable() = default;
};

#endif
