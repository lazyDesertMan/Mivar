#ifndef MIVAR_RULE_H
#define MIVAR_RULE_H

#include "IObserver.h"
#include "MivarRelation.h"
#include <iostream>

class MivarRule : public IObservable {
    struct MivarRuleLogic : public IObserver {
        MivarRule* parent;
        void handle() { parent->sendEvent(); }
    };

    std::shared_ptr<MivarRelation> m_relation;
    std::shared_ptr<MivarRuleLogic> m_rule;
public:
    MivarRule(const std::shared_ptr<MivarRelation>& relation);
    MivarRule(MivarRule&) = delete;
    MivarRule(MivarRule&&) = delete;
    MivarRule& operator=(MivarRule&) = delete;
    ~MivarRule();

    bool isCorrect();
};

#endif
