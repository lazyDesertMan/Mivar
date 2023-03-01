#include "MivarClass.h"

MivarClass::MivarClass(const QString& id, const QString& name, const QString& description) {
    m_id = id;
    m_name = name;
    m_description = description;
}

const std::vector<std::shared_ptr<MivarParam>>& MivarClass::params() const noexcept {
    return m_params;
}

void MivarClass::addParam(const std::shared_ptr<MivarParam>& param) {
    if (!contains(param->id())) {
        m_params.push_back(param);
        sendEvent(EventCode::EC_PARAMS_UPDATE);
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
        sendEvent(EventCode::EC_SUBCLASSES_UPDATE);
    }
}

bool MivarClass::contains(const QString& id) const noexcept {
    size_t idx = 0;
    if (m_id != id) {
        while (idx < m_subclasses.size() && !m_subclasses[idx]->contains(id))
            idx++;
        if (idx == m_subclasses.size()) {
            idx = 0;
            while (idx < m_params.size() && m_params[idx]->id() != id)
                idx++;
            return idx != m_params.size();
        }
    }
    return true;
}

void MivarClass::addRule(std::shared_ptr<MivarRule> rule) {
    if (!contains(rule->id())) {
        m_rules.push_back(rule);
        sendEvent(EventCode::EC_RULES_UPDATE);
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

const MivarParam::ParamType MivarParam::type() const noexcept {
    return m_type;
}

void MivarParam::setType(const QString& type) {
    if (type == "double") {
        m_type = ParamType::PT_DOUBLE;
        sendEvent();
    }
    else if (type == "string") {
        m_type = ParamType::PT_STRING;
        sendEvent();
    }
}

void MivarParam::setType(const ParamType type) {
    if (type != ParamType::PT_NONE) {
        m_type = type;
        sendEvent();
    }
}

bool MivarParam::isCorrect() const noexcept {
    return m_id.size() > 0 && m_name.size() > 0 && m_type != ParamType::PT_NONE;
}
