#include "MivarRule.h"

MivarRule::MivarRule(const std::shared_ptr<MivarRelation>& relation) : m_relation(relation) {
    m_rule = std::make_shared<MivarRuleLogic>();
    m_rule->parent = this;
    m_relation->addObserver(m_rule);
}

MivarRule::~MivarRule() {
    m_relation->removeObserver(m_rule);
}

bool MivarRule::isCorrect() {
    return true;
}
