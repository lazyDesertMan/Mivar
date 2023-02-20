#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <QString>
#include <memory>
#include "MivarObject.h"
#include "MivarRelation.h"

/**
 * @brief Параметр класса
 */
class MivarParam : public MivarObject {
    QString m_type;
public:
    class InvalidTypeException {};
    
    const QString& type() const noexcept;
    void setType(const QString& type);
};

/**
 * @brief Класс
 */
class MivarClass : public MivarObject {
    std::vector<MivarParam> m_params;     //!< Параметры класса
    std::vector<MivarClass> m_subclasses; //!< Подклассы

public:
    MivarClass(const QString& id = "", const QString& name = "", const QString& description = "");

    const std::vector<MivarParam>& params() const noexcept;
    const std::vector<MivarClass>& subclasses() const noexcept;
};

/**
 * @brief Модель
 */
class MivarModel : public MivarObject {
    std::vector<MivarClass> m_classes;
    std::vector<std::shared_ptr<MivarRelation>> m_relations;

public:
    MivarModel(const QString& id = "", const QString& name = "", const QString& description = "");

    const std::vector<MivarClass>& classes() const noexcept;
    bool addClass(const MivarClass& modelClass);
    void removeClass(const QString& id);

    const std::vector<std::shared_ptr<MivarRelation>>& relations() const noexcept;
    bool addRelation(const std::shared_ptr<MivarRelation>& relation);
    void removeRelation(const QString& id);
};

#endif
