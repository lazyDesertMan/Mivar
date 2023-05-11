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
    QString m_savePath;

public:
    enum EventCode : int16_t { EC_RELATION_ADD = 16, EC_RELATION_REMOVE };

    MivarModel(const QString& id = "", const QString& name = "", const QString& description = "");

    std::shared_ptr<MivarClass>& modelClass();
    const std::shared_ptr<MivarClass>& modelClass() const;
    void setModelClass(const std::shared_ptr<MivarClass>& mivarClass);

    const bool contains(const QString& id) const;
    const std::vector<std::shared_ptr<MivarRelation>>& relations() const noexcept;
    const bool containsRelation(const QString& id) const;
    bool addRelation(const std::shared_ptr<MivarRelation> relation);
    const std::shared_ptr<MivarRelation>& getRelation(const QString& id) const;
    std::shared_ptr<MivarRelation>& getRelation(const QString& id);
    void setSavePath(const QString& path);
    const QString& savePath();
    void removeRelation(const QString& id);
};

#endif
