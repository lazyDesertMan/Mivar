#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <QString>
#include <memory>
#include "MivarObject.h"
#include "MivarRelation.h"
#include "MivarClass.h"
#include "IObservable.h"

/**
 * @brief Миварная модель
 */
class MivarModel : public MivarObject {
    std::shared_ptr<MivarClass> m_rootClass;
    std::vector<std::shared_ptr<MivarRelation>> m_relations;

public:
    enum EventCode : int16_t { EC_RELATION_ADD = 16, EC_RELATION_REMOVE };

    MivarModel(const QString& id = "", const QString& name = "", const QString& description = "");

    std::shared_ptr<MivarClass>& modelClass();
    const std::shared_ptr<MivarClass>& modelClass() const;
    void setModelClass(const std::shared_ptr<MivarClass>& mivarClass);

    const std::vector<std::shared_ptr<MivarRelation>>& relations() const noexcept;
    bool addRelation(const std::shared_ptr<MivarRelation> relation);
    const std::shared_ptr<MivarRelation>& getRelation(const QString& id) const;
    std::shared_ptr<MivarRelation>& getRelation(const QString& id);
    void removeRelation(const QString& id);
};

#endif
