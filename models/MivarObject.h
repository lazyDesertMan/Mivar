#ifndef MIVAR_OBJECT_H
#define MIVAR_OBJECT_H

#include <QString>
#include "IObservable.h"

/**
 * @brief Общая структура объектов модели
 */
class MivarObject : public IObservable {
protected:
    QString m_id;           //!< UUID
    QString m_name;         //!< Имя
    QString m_description;  //!< Описание

public:
    enum EventCode : int16_t { EC_NAME_UPDATE = 1, EC_DESCRIPTION_UPDATE };

    const QString& id() const noexcept;
    const QString& name() const noexcept;
    const QString& description() const noexcept;

    void setId(const QString& id);
    void setName(const QString& name);
    void setDescription(const QString& description);
};

#endif
