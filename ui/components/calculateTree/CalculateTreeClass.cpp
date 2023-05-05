#include <QDebug>
#include <QBoxLayout>
#include "CalculateTreeClass.h"

void CalculateTreeClassLabel::CalculateClassObserver::handle(int16_t code) {
    switch (code)
    {
    case MivarObject::EventCode::EC_NAME_UPDATE:
        parent->update();
        break;
    case MivarClass::EventCode::EC_PARAM_ADD:
        parent->paramAdded();
        break;
    case MivarClass::EventCode::EC_PARAM_REMOVE:
        parent->paramRemoved();
        break;
    case MivarClass::EventCode::EC_SUBCLASS_ADD:
        parent->subclassAdded();
        break;
    case MivarClass::EventCode::EC_SUBCLASS_REMOVE:
        parent->subclassRemoved();
        break;
    default:
        break;
    }
}

void CalculateTreeClassLabel::paramAdded() {
    emit onParamAdd(m_class);
}

void CalculateTreeClassLabel::paramRemoved() {
    emit onParamRemove(m_class);
}

void CalculateTreeClassLabel::subclassAdded() {
    emit onSubclassAdd(m_class);
}

void CalculateTreeClassLabel::subclassRemoved() {
    emit onSubclassRemove(m_class);
}

void CalculateTreeClassLabel::update() {
    m_name->setText(m_class->name());
    m_name->setMinimumWidth(m_class->name().size() * 10);
}

std::shared_ptr<MivarClass> CalculateTreeClassLabel::getClass() {
    return m_class;
}

CalculateTreeClassLabel::CalculateTreeClassLabel(std::shared_ptr<MivarClass> observedClass) : m_class(observedClass) {
    m_observer = std::make_shared<CalculateClassObserver>();
    m_observer->parent = this;
    m_class->addObserver(m_observer);

    QVBoxLayout* layout = new QVBoxLayout(this);
    setLayout(layout);
    m_name = new QLabel(this);
    layout->addWidget(m_name);
    layout->setAlignment(Qt::AlignVCenter);
    
    update();
}

CalculateTreeClassLabel::~CalculateTreeClassLabel() {
    m_class->removeObserver(m_observer);
}
