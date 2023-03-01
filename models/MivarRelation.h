#ifndef MIVAR_RELATION_H
#define MIVAR_RELATION_H

#include <vector>
#include <QString>
#include "IObservable.h"
#include "MivarObject.h"

/**
 * @brief Отношение
 */
class MivarRelation : public MivarObject {
public:
    class InvalidTypeException {};
    
    struct RelationParameter {
        QString name;
        QString type;
        RelationParameter(const QString& name, const QString& type);
    };

    /**
     * @brief Сведения об ошибке
     */
    struct ErrorData {
        int     line;  //!< Номер ошибочной строки
        QString msg;   //!< Сообщение об ошибке
    };

protected:
    QString m_code;
    std::vector<RelationParameter> m_inputs;
    std::vector<RelationParameter> m_outputs;

public:
    MivarRelation(const QString& id = "", const QString& name = "", const QString& description = "", const QString& code = "");

    virtual const QString type() const noexcept = 0;
    
    const QString& code() const noexcept;
    virtual void setCode(const QString& code);
    const std::vector<RelationParameter>& inputs() const noexcept;
    bool addInput(const RelationParameter& param);
    const std::vector<RelationParameter>& outputs() const noexcept;
    bool addOutput(const RelationParameter& param);
    void removeParam(const QString& paramName);
    bool containsParam(const QString& name) const noexcept;
    void renameParam(const QString& paramName, const QString& newParamName);
    void setParamType(const QString& paramName, const QString& type);

    virtual QString toJSFunction() const = 0;
    
    /**
     * @brief Получение информации об ошибке в коде
     * 
     * @return ErrorData Данные об ошибке. Если ошибки нет, полю line будет присвоено значение -1
     */
    virtual ErrorData errorDetails() const = 0;

    virtual ~MivarRelation() = default;
};

class MivarFunctionRelation : public MivarRelation {
public:
    virtual QString toJSFunction() const;
    virtual ErrorData errorDetails() const;
    virtual const QString type() const noexcept;
    virtual ~MivarFunctionRelation() = default;
};

#endif
