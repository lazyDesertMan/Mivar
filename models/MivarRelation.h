#ifndef MIVAR_RELATION_H
#define MIVAR_RELATION_H

#include <vector>
#include <QString>
#include "MivarObject.h"

/**
 * @brief Отношение
 */
class MivarRelation : public MivarObject {
public:
    class InvalidTypeException {};
    
    class RelationParameter {
        QString m_name;
        QString m_type;
    public:
        RelationParameter(const QString& name, const QString& type);

        const QString& name() const noexcept;
        const QString& type() const noexcept;
        void setName(const QString& name);
        void setType(const QString& type);
    };

protected:
    QString m_code;
    std::vector<RelationParameter> m_inputs;
    std::vector<RelationParameter> m_outputs;

public:
    MivarRelation(const QString& id = "", const QString& name = "", const QString& description = "");

    virtual const QString& type() const noexcept = 0;
    
    const QString& code() const noexcept;
    void setCode(const QString& code);
    const std::vector<RelationParameter>& inputs() const noexcept;
    bool addInput(const RelationParameter& param);
    const std::vector<RelationParameter>& outputs() const noexcept;
    bool addOutput(const RelationParameter& param);
    void removeParam(const QString& paramName);
    bool containsParam(const QString& name) const noexcept;

    virtual QString toJSFunction() const = 0;

    virtual ~MivarRelation() = default;
};

class MivarFunctionRelation : public MivarRelation {
public:
    virtual QString toJSFunction() const;
    virtual const QString& type() const noexcept;
    virtual ~MivarFunctionRelation() = default;
};

#endif
