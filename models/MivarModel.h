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
 * @brief Модель
 */
class MivarModel : public MivarObject, public IObservable {
    MivarClass m_rootClass;
    std::vector<std::shared_ptr<MivarRelation>> m_relations;

public:
    MivarModel(const QString& id = "", const QString& name = "", const QString& description = "");

    MivarClass& modelClass();
    void setModelClass(const MivarClass& mivarClass);

    const std::vector<std::shared_ptr<MivarRelation>>& relations() const noexcept;
    bool addRelation(const std::shared_ptr<MivarRelation>& relation);
    void removeRelation(const QString& id);
};

#endif
