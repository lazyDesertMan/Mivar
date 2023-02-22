#ifndef SINGLE_JS_ENGINE_H
#define SINGLE_JS_ENGINE_H

#include <QJSEngine>

class SingleJSEngine {
public:
    SingleJSEngine() = delete;
    SingleJSEngine(SingleJSEngine&) = delete;
    SingleJSEngine(SingleJSEngine&&) = delete;
    SingleJSEngine& operator=(SingleJSEngine&) = delete;
    SingleJSEngine& operator=(SingleJSEngine&&) = delete;

    static QJSEngine& engine() {
        static QJSEngine inst;
        return inst;
    }
};

#endif
