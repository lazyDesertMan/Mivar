#include <iostream>
#include "MivarModel.h"

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

MivarModel::MivarModel(const QString& id, const QString& name, const QString& description) {
    m_id = id;
    m_name = name;
    m_description = description;
}

const std::vector<MivarClass>& MivarModel::classes() const noexcept {
    return m_classes;
}

bool MivarModel::addClass(const MivarClass& modelClass) {
    size_t idx = 0;
    while (idx < m_classes.size() && m_classes[idx].id() != modelClass.id())
        idx++;
    if (idx == m_classes.size()) {
        m_classes.push_back(modelClass);
        return true;
    }
    return false;
}

void MivarModel::removeClass(const QString& id) {
    size_t idx = 0;
    while (idx < m_classes.size() && m_classes[idx].id() != id)
        idx++;
    if (idx != m_classes.size())
        m_classes.erase(m_classes.begin() + idx);
}

const std::vector<std::shared_ptr<MivarRelation>>& MivarModel::relations() const noexcept {
    return m_relations;
}

bool MivarModel::addRelation(const std::shared_ptr<MivarRelation>& relation) {
    size_t idx = 0;
    while (idx < m_relations.size() && m_relations[idx]->id() != relation->id())
        idx++;
    if (idx == m_relations.size()) {
        m_relations.push_back(relation);
        return true;
    }
    return false;
}

void MivarModel::removeRelation(const QString& id) {
    size_t idx = 0;
    while (idx < m_relations.size() && m_relations[idx]->id() != id)
        idx++;
    if (idx != m_relations.size())
        m_relations.erase(m_relations.begin() + idx);
}

const QString& MivarParam::type() const noexcept {
    return m_type;
}

void MivarParam::setType(const QString& type) {
    if (type == "double" || type == "string")
        m_type = type;
    else
        throw InvalidTypeException();
}

MivarRelation::RelationParameter::RelationParameter(const QString& name, const QString& type) {
    setName(name);
    setType(type);
}

const QString &MivarRelation::RelationParameter::name() const noexcept
{
    return m_name;
}

const QString& MivarRelation::RelationParameter::type() const noexcept {
    return m_type;
}

void MivarRelation::RelationParameter::setName(const QString& name) {
    m_name = name;
}

void MivarRelation::RelationParameter::setType(const QString& type) {
    if (type == "double" || type == "string")
        m_type = type;
    else
        throw MivarRelation::InvalidTypeException();
}
