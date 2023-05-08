#include <iostream>
#include <stdexcept>
#include "MivarModel.h"

MivarModel::MivarModel(const QString& id, const QString& name, const QString& description) {
    m_id = id;
    m_name = name;
    m_description = description;
}

std::shared_ptr<MivarClass>& MivarModel::modelClass() {
    return m_rootClass;
}

const std::shared_ptr<MivarClass>& MivarModel::modelClass() const {
    return m_rootClass;
}

void MivarModel::setModelClass(const std::shared_ptr<MivarClass>& mivarClass) {
    m_rootClass = mivarClass;
}

const bool MivarModel::contains(const QString &id) const {
    return m_rootClass->contains(id);
}

const std::vector<std::shared_ptr<MivarRelation>>& MivarModel::relations() const noexcept {
    return m_relations;
}

const bool MivarModel::containsRelation(const QString &id) const {
    for (size_t idx = 0; idx < m_relations.size(); idx++)
        if(m_relations[idx]->id() == id)
            return true;
    return false;
}

bool MivarModel::addRelation(const std::shared_ptr<MivarRelation> relation) {
    size_t idx = 0;
    while (idx < m_relations.size() && m_relations[idx]->id() != relation->id())
        idx++;
    if (idx == m_relations.size()) {
        m_relations.push_back(relation);
        sendEvent(EventCode::EC_RELATION_ADD);
        return true;
    }
    return false;
}

const std::shared_ptr<MivarRelation>& MivarModel::getRelation(const QString& id) const {
    for (size_t idx = 0; idx < m_relations.size(); idx++) {
        if (m_relations[idx]->id() == id)
            return m_relations[idx];
    }
    throw std::invalid_argument("Отношение не существует (" + id.toStdString() + ")");
}

std::shared_ptr<MivarRelation>& MivarModel::getRelation(const QString& id) {
    for (size_t idx = 0; idx < m_relations.size(); idx++) {
        if (m_relations[idx]->id() == id)
            return m_relations[idx];
    }
    throw std::invalid_argument("Отношение не существует (" + id.toStdString() + ")");
}

void MivarModel::removeRelation(const QString& id) {
    size_t idx = 0;
    while (idx < m_relations.size() && m_relations[idx]->id() != id)
        idx++;
    if (idx != m_relations.size()) {
        m_rootClass->removeRelation(m_relations[idx]->id());
        m_relations.erase(m_relations.begin() + idx);
        sendEvent(EventCode::EC_RELATION_REMOVE);
    }
}
