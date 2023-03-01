#include "IObservable.h"

void IObservable::sendEvent(int16_t code) {
    for(const std::shared_ptr<IObserver>& observer : m_observers)
        observer->handle(code);
}

void IObservable::addObserver(std::shared_ptr<IObserver> observer) {
    m_observers.push_back(observer);
}

void IObservable::removeObserver(std::shared_ptr<IObserver> observer) {
    for(size_t i = 0; i < m_observers.size(); i++)
        if (m_observers[i] == observer) {
            m_observers.erase(m_observers.begin() + i);
            return;
        }
}
