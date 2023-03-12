#include "MivarClass.h"

void MivarClass::remove() {
    for(size_t i = 0; i < m_rules.size(); i++)
        m_rules[i];
}

MivarClass::MivarClass(const QString &id, const QString &name, const QString &description) {
    m_id = id;
    m_name = name;
    m_description = description;
}

MivarClass::~MivarClass() {
    for(size_t i = 0; i < m_rules.size(); i++) {
        m_rules[i]->unbindRelation();
    }
}

const std::vector<std::shared_ptr<MivarParam>>& MivarClass::params() const noexcept {
    return m_params;
}

void MivarClass::addParam(const std::shared_ptr<MivarParam>& param) {
    if (!contains(param->id())) {
        m_params.push_back(param);
        sendEvent(EventCode::EC_PARAM_ADD);
    }
}

const std::vector<std::shared_ptr<MivarClass>>& MivarClass::subclasses() const noexcept {
    return m_subclasses;
}

const std::vector<std::shared_ptr<MivarRule>>& MivarClass::rules() const noexcept {
    return m_rules;
}

void MivarClass::addSubclass(const std::shared_ptr<MivarClass>& subclass) {
    if (!contains(subclass->m_id)) {
        m_subclasses.push_back(subclass);
        sendEvent(EventCode::EC_SUBCLASS_ADD);
    }
}

bool MivarClass::contains(const QString& id) const noexcept {
    if (m_id != id) {
        size_t idx = 0;
        while (idx < m_subclasses.size() && !m_subclasses[idx]->contains(id))
            idx++;
        if (idx == m_subclasses.size()) {
            idx = 0;
            while (idx < m_params.size() && m_params[idx]->id() != id)
                idx++;
            if (idx == m_params.size()) {
                idx = 0;
                while (idx < m_rules.size() && m_rules[idx]->id() != id)
                    idx++;
                return idx != m_rules.size();
            }
        }
    }
    return true;
}

bool MivarClass::removeById(const QString& id) {
    if (m_id != id) {
        size_t idx = 0;
        while (idx < m_subclasses.size() && m_subclasses[idx]->m_id != id)
            idx++;
        if (idx == m_subclasses.size()) {
            idx = 0;
            while (idx < m_params.size() && m_params[idx]->id() != id)
                idx++;
            if (idx == m_params.size()) {
                idx = 0;
                while (idx < m_rules.size() && m_rules[idx]->id() != id)
                    idx++;
                if (idx == m_rules.size()) {
                    idx = 0;
                    while (idx < m_subclasses.size()) {
                        if(m_subclasses[idx]->removeById(id))
                            return true;
                        idx++;
                    }
                } else {
                    m_rules.erase(m_rules.begin() + idx);
                    sendEvent(EventCode::EC_RULE_REMOVE);
                    return true;
                }
            } else {
                m_params.erase(m_params.begin() + idx);
                sendEvent(EventCode::EC_PARAM_REMOVE);
                return true;
            }
        } else {
            m_subclasses.erase(m_subclasses.begin() + idx);
            sendEvent(EventCode::EC_SUBCLASS_REMOVE);
            return true;
        }
    }
    return false;
}

void MivarClass::removeRel(const QString &id)
{
    for(size_t i = 0; i < m_rules.size(); i++){
        if(m_rules[i]->getBindetRelation()->id() == id){
            m_rules[i]->unbindRelation();
            m_rules.erase(m_rules.begin() + i);
        }
    }
    for(size_t i = 0; i < m_subclasses.size(); i++){
        m_subclasses[i]->removeRel(id);
    }
}

void MivarClass::addRule(std::shared_ptr<MivarRule> rule) {
    if (!contains(rule->id())) {
        m_rules.push_back(rule);
        sendEvent(EventCode::EC_RULE_ADD);
    }
}

void MivarClass::removeRule(const QString& id) {
    for(size_t i = 0; i < m_rules.size(); i++)
        if(m_rules[i]->id() == id) {
            m_rules[i]->unbindRelation();
            m_rules.erase(m_rules.begin() + i);
            sendEvent(EventCode::EC_RULE_REMOVE);
            return;
        }
}

bool MivarClass::paramContains(const QString& id) const noexcept {
    size_t idx = 0;
    while (idx < m_params.size() && m_params[idx]->id() != id)
        idx++;
    if (idx == m_params.size()) {
        idx = 0;
        while (idx < m_subclasses.size() && !m_subclasses[idx]->paramContains(id))
            idx++;
        return idx != m_subclasses.size();
    }
    return true;
}

const int16_t MivarParam::type() const noexcept {
    return m_type;
}

void MivarParam::setType(const QString& type) {
    if (type == "double") {
        m_type = PARAM_TYPE_DOUBLE;
        sendEvent(EventCode::EC_TYPE_CHANGE);
    }
    else if (type == "string") {
        m_type = PARAM_TYPE_STRING;
        sendEvent(EventCode::EC_TYPE_CHANGE);
    }
}

void MivarParam::setType(const int16_t type) {
    if (type == PARAM_TYPE_DOUBLE || type == PARAM_TYPE_STRING) {
        m_type = type;
        sendEvent(EventCode::EC_TYPE_CHANGE);
    }
}

bool MivarParam::isCorrect() const noexcept {
    return m_id.size() > 0 && m_name.size() > 0 && (m_type == PARAM_TYPE_DOUBLE || m_type == PARAM_TYPE_STRING);
}
