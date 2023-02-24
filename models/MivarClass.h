#ifndef MIVAR_CLASS_H
#define MIVAR_CLASS_H

#include <vector>
#include <QString>
#include "MivarObject.h"

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
    void addSubclass(const MivarClass& subclass);
    bool contains(const QString& id) const noexcept;
};

#endif
