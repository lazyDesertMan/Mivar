#include <QDebug>
#include "TreeClassDetail.h"

const std::shared_ptr<MivarClass> TreeClassDetail::getClass() const {
    return m_class;
}

QLabel* TreeClassDetail::name() {
    return m_name;
}

void TreeClassDetail::update() {
    m_name->setText(m_class->name());
}

TreeClassDetail::TreeClassDetail(std::shared_ptr<MivarClass> observedClass) {
    m_observer = std::make_shared<ClassObserver>();
    m_observer->parent = this;
    m_class = observedClass;
    m_class->addObserver(m_observer);
    m_name = new QLabel();
    update();
}

TreeClassDetail::~TreeClassDetail() {
    m_class->removeObserver(m_observer);
    delete m_name;
}

void TreeClassDetail::ClassObserver::handle(int16_t code) {
    switch (code)
    {
    case MivarObject::EventCode::EC_NAME_UPDATE:
        parent->update();
        break;
    default:
        break;
    }
}
