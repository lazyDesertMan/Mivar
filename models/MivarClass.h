#ifndef MIVAR_CLASS_H
#define MIVAR_CLASS_H

#include <vector>
#include <QString>
#include <QRegExp>
#include "MivarObject.h"
#include "MivarRule.h"
#include "IObservable.h"

/**
 * @brief Параметр класса
 */
class MivarParam : public MivarObject {
public:
    static const QRegExp doubleExp;
    static constexpr int16_t PARAM_TYPE_NONE   = 0;
    static constexpr int16_t PARAM_TYPE_DOUBLE = 1;
    static constexpr int16_t PARAM_TYPE_STRING = 2;

    enum EventCode : int16_t {
        EC_TYPE_CHANGE = 16
    };

    class InvalidTypeException {};

    MivarParam() : m_type(PARAM_TYPE_NONE) {}

    const int16_t type() const noexcept;
    void setType(const QString& type);
    void setType(const int16_t type);
    const QString& defaultValue() const noexcept;
    void setDefaultValue(const QString& val);
    bool isCorrect() const noexcept;

protected:
    int16_t m_type;
    QString m_defaultValue;
};

/**
 * @brief Класс
 */
class MivarClass : public MivarObject {
    std::vector<std::shared_ptr<MivarParam>> m_params;     //!< Параметры класса
    std::vector<std::shared_ptr<MivarClass>> m_subclasses; //!< Подклассы
    std::vector<std::shared_ptr<MivarRule>>  m_rules;      //!< Правила

    void remove();

public:
    enum EventCode : int16_t {
        EC_PARAM_ADD = 16,
        EC_SUBCLASS_ADD,
        EC_RULE_ADD,
        EC_PARAM_REMOVE,
        EC_SUBCLASS_REMOVE,
        EC_RULE_REMOVE
    };

    MivarClass(const QString& id = "", const QString& name = "", const QString& description = "");
    MivarClass(MivarClass&) = delete;
    MivarClass(MivarClass&&) = delete;
    MivarClass& operator=(MivarClass&) = delete;
    MivarClass& operator=(MivarClass&&) = delete;
    virtual ~MivarClass();

    const std::vector<std::shared_ptr<MivarParam>>& params() const noexcept;
    void addParam(const std::shared_ptr<MivarParam>& param);
    bool paramContains(const QString& id) const noexcept;

    const std::vector<std::shared_ptr<MivarClass>>& subclasses() const noexcept;
    void addSubclass(const std::shared_ptr<MivarClass>& subclass);

    const std::vector<std::shared_ptr<MivarRule>>& rules() const noexcept;
    void addRule(std::shared_ptr<MivarRule> rule);
    bool removeRule(const QString& id);
    
    bool contains(const QString& id) const noexcept;
    bool removeById(const QString& id);
    void removeRelation(const QString& id);
};

#endif
