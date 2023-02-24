#include <iostream>
#include "MivarModel.h"

MivarModel::MivarModel(const QString& id, const QString& name, const QString& description) {
    m_id = id;
    m_name = name;
    m_description = description;
}

MivarClass& MivarModel::modelClass() {
    return m_rootClass;
}

void MivarModel::setModelClass(const MivarClass &mivarClass) {
    m_rootClass = mivarClass;
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
        sendEvent();
        return true;
    }
    return false;
}

void MivarModel::removeRelation(const QString& id) {
    size_t idx = 0;
    while (idx < m_relations.size() && m_relations[idx]->id() != id)
        idx++;
    if (idx != m_relations.size()) {
        m_relations.erase(m_relations.begin() + idx);
        sendEvent();
    }
}
