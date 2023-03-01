#ifndef MIVAR_CLASS_H
#define MIVAR_CLASS_H

#include <vector>
#include <QString>
#include "MivarObject.h"
#include "MivarRule.h"
#include "IObservable.h"

/**
 * @brief Параметр класса
 */
class MivarParam : public MivarObject {
public:
    enum ParamType { PT_NONE, PT_DOUBLE, PT_STRING };

    class InvalidTypeException {};

    MivarParam() : m_type(ParamType::PT_NONE) {}

    const ParamType type() const noexcept;
    void setType(const QString& type);
    void setType(const ParamType type);
    bool isCorrect() const noexcept;

protected:
    ParamType m_type;
};

/**
 * @brief Класс
 */
class MivarClass : public MivarObject {
    std::vector<std::shared_ptr<MivarParam>> m_params;     //!< Параметры класса
    std::vector<std::shared_ptr<MivarClass>> m_subclasses; //!< Подклассы
    std::vector<std::shared_ptr<MivarRule>>  m_rules;      //!< Правила

public:
    enum EventCode : int16_t { EC_PARAMS_UPDATE = 16, EC_SUBCLASSES_UPDATE, EC_RULES_UPDATE };

    MivarClass(const QString& id = "", const QString& name = "", const QString& description = "");

    const std::vector<std::shared_ptr<MivarParam>>& params() const noexcept;
    void addParam(const std::shared_ptr<MivarParam>& param);
    bool paramContains(const QString& id) const noexcept;

    const std::vector<std::shared_ptr<MivarClass>>& subclasses() const noexcept;
    void addSubclass(const std::shared_ptr<MivarClass>& subclass);

    const std::vector<std::shared_ptr<MivarRule>>& rules() const noexcept;
    void addRule(std::shared_ptr<MivarRule> rule);
    
    bool contains(const QString& id) const noexcept;
};

#endif
