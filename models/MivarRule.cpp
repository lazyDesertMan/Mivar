#include <QDebug>
#include "MivarRule.h"

void MivarRule::bindRelation(std::shared_ptr<MivarRelation> relation) {
    if (m_isBindet)
        m_relation->removeObserver(shared_from_this());
    m_relation = relation;
    relation->addObserver(shared_from_this());
    m_isBindet = true;
    syncWithRelation();
}

void MivarRule::unbindRelation() {
    if(m_isBindet) {
        m_relation->removeObserver(shared_from_this());
        m_isBindet = false;
    }
}

const std::shared_ptr<MivarRelation> MivarRule::getBindetRelation() const {
    return m_relation;
}

QString MivarRule::paramID(const QString& paramName) {
    if(m_params.find(paramName) != m_params.end())
        return m_params[paramName];
    return "";
}

bool MivarRule::isCorrect() {
    return true;
}

bool MivarRule::bindParam(const QString& paramName, const QString& paramId) {
    if (m_params.find(paramName) != m_params.end()) {
        m_params[paramName] = paramId;
        sendEvent();
        return true;
    }
    return false;
}

const std::map<QString, QString>& MivarRule::params() const noexcept {
    return m_params;
}

const std::vector<QString> MivarRule::inputsId() {
    std::vector<QString> inputs;
    for(const MivarRelation::RelationParameter& param : m_relation->inputs())
        inputs.push_back(m_params[param.name]);
    return inputs;
}

const std::vector<QString> MivarRule::outputsId() {
    std::vector<QString> outputs;
    for(const MivarRelation::RelationParameter& param : m_relation->outputs())
        outputs.push_back(m_params[param.name]);
    return outputs;
}

void MivarRule::syncWithRelation() {
    for (std::pair<const QString, QString> param : m_params)
        if (!m_relation->containsParam(param.first))
            m_params.erase(param.first);
    for (const MivarRelation::RelationParameter& inpParam : m_relation->inputs())
        if (m_params.find(inpParam.name) == m_params.end())
            m_params[inpParam.name] = "";
    for (const MivarRelation::RelationParameter& outParam : m_relation->outputs())
        if (m_params.find(outParam.name) == m_params.end())
            m_params[outParam.name] = "";
}

void MivarRule::handle(int16_t code) {
    syncWithRelation();
    sendEvent();
}

MivarRule::~MivarRule() {
    if(m_isBindet)
        m_relation->removeObserver(shared_from_this());
}
