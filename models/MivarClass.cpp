#include "MivarClass.h"

MivarClass::MivarClass(const QString& id, const QString& name, const QString& description) {
    m_id = id;
    m_name = name;
    m_description = description;
}

const std::vector<MivarParam>& MivarClass::params() const noexcept {
    return m_params;
}

const std::vector<MivarClass> &MivarClass::subclasses() const noexcept {
    return m_subclasses;
}

const std::vector<std::shared_ptr<MivarRule>>& MivarClass::rules() const noexcept {
    return m_rules;
}

void MivarClass::addSubclass(const MivarClass& subclass) {
    if (!contains(subclass.m_id))
        m_subclasses.push_back(subclass);
}

bool MivarClass::contains(const QString& id) const noexcept {
    size_t idx = 0;
    if (m_id != id) {
        while (idx < m_subclasses.size() && !m_subclasses[idx].contains(id))
            idx++;
        if (idx == m_subclasses.size()) {
            idx = 0;
            while (idx < m_params.size() && m_params[idx].id() != id)
                idx++;
            return idx != m_params.size();
        }
    }
    return true;
}

void MivarClass::addRule(std::shared_ptr<MivarRule> rule) {
    m_rules.push_back(rule);
}

bool MivarClass::paramContains(const QString& id) const noexcept {
    size_t idx = 0;
    while (idx < m_params.size() && m_params[idx].id() != id)
        idx++;
    if (idx == m_params.size()) {
        idx = 0;
        while (idx < m_subclasses.size() && !m_subclasses[idx].paramContains(id))
            idx++;
        return idx != m_subclasses.size();
    }
    return true;
}

const QString& MivarParam::type() const noexcept {
    return m_type;
}

void MivarParam::setType(const QString& type) {
    if (type == "double" || type == "string")
        m_type = type;
}
