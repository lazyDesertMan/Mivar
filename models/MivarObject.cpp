#include <QDebug>
#include "MivarObject.h"

const QString& MivarObject::id() const noexcept {
    return m_id;
}

const QString& MivarObject::name() const noexcept {
    return m_name;
}

const QString& MivarObject::description() const noexcept {
    return m_description;
}

void MivarObject::setId(const QString& id) {
    m_id = id;
}

void MivarObject::setName(const QString& name) {
    m_name = name;
    sendEvent(EventCode::EC_NAME_UPDATE);
}

void MivarObject::setDescription(const QString& description) {
    m_description = description;
    sendEvent(EventCode::EC_DESCRIPTION_UPDATE);
}

MivarObject::~MivarObject() {
    // TODO: Вывод имён удаляемых объектов, на время разработки (06.03.2023)
    qDebug() << "Remove " << m_name;
}
