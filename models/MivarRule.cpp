#include "MivarRule.h"

MivarRule::MivarRule(const std::shared_ptr<MivarRelation>& relation) { 
    m_rule = std::make_shared<MivarRuleLogic>();
    m_rule->parent = this;
    m_rule->relation = relation;
    m_rule->syncWithRelation();
    m_rule->relation->addObserver(m_rule);
}

MivarRule::~MivarRule() {
    m_rule->relation->removeObserver(m_rule);
}

bool MivarRule::isCorrect() {
    return true;
}

bool MivarRule::bindParam(const QString& paramName, const QString& paramId) {
    if (m_rule->params.find(paramName) != m_rule->params.end()) {
        m_rule->params[paramName] = paramId;
        sendEvent();
        return true;
    }
    return false;
}

const std::map<QString, QString>& MivarRule::params() const noexcept {
    return m_rule->params;
}

void MivarRule::MivarRuleLogic::syncWithRelation() {
    for (std::pair<const QString, QString> param : params)
        if (!relation->containsParam(param.first))
            params.erase(param.first);
    for (const MivarRelation::RelationParameter& inpParam : relation->inputs())
        if (params.find(inpParam.name) == params.end())
            params[inpParam.name] = "";
    for (const MivarRelation::RelationParameter& outParam : relation->outputs())
        if (params.find(outParam.name) == params.end())
            params[outParam.name] = "";
}

void MivarRule::MivarRuleLogic::handle(int16_t code) {
    syncWithRelation();
    parent->sendEvent();
}
