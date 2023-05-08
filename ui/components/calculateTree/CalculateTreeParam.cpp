#include <QBoxLayout>
#include "CalculateTreeParam.h"

void CalculateTreeParamLabel::CalculateTreeParamObserver::handle(int16_t code) {
    if (code == MivarObject::EventCode::EC_NAME_UPDATE)
        parent->update();
}

void CalculateTreeParamLabel::update() {
    m_label->setText(m_param->name());
    emit onNameUpdate(m_param);
}

CalculateTreeParamLabel::CalculateTreeParamLabel(std::shared_ptr<MivarParam> observedParam) : m_param(observedParam) {
    m_observer = std::make_shared<CalculateTreeParamObserver>();
    m_observer->parent = this;
    m_param->addObserver(m_observer);

    QVBoxLayout* layout = new QVBoxLayout(this);
    setLayout(layout);
    m_label = new QLabel(this);
    m_label->setText(m_param->name());
    layout->addWidget(m_label);
    layout->setAlignment(Qt::AlignVCenter);
}

CalculateTreeParamLabel::~CalculateTreeParamLabel() {
    m_param->removeObserver(m_observer);
}
