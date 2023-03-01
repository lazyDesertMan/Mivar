#include <QDebug>
#include <QBoxLayout>
#include "TreeParamDetail.h"

const std::shared_ptr<MivarParam> TreeParamDetail::getParam() const {
    return m_param;
}

QWidget* TreeParamDetail::name() {
    return m_container;
}

void TreeParamDetail::update() {
    m_name->setText(m_param->name());
}

TreeParamDetail::TreeParamDetail(std::shared_ptr<MivarParam> observedParam) {
    m_observer = std::make_shared<ParamObserver>();
    m_observer->parent = this;
    m_param = observedParam;
    m_param->addObserver(m_observer);
    
    m_container = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(m_container);
    m_container->setLayout(layout);
    m_name = new QLabel(m_container);
    m_name->setText(m_param->name());

    m_icon = new QLabel(m_container);
    QPixmap classIcon("://image/res/icons/param.svg");
    m_icon->setPixmap(classIcon);
    m_icon->setScaledContents(true);
    m_icon->setFixedSize(16, 16);

    layout->addWidget(m_icon);
    layout->addWidget(m_name);
    layout->setStretch(0, 0);
    layout->setStretch(1, 1);
    layout->setAlignment(Qt::AlignCenter);

    update();
}

TreeParamDetail::~TreeParamDetail() {
    m_param->removeObserver(m_observer);
    delete m_container;
}

void TreeParamDetail::ParamObserver::handle(int16_t code) {
    switch (code)
    {
    case MivarObject::EventCode::EC_NAME_UPDATE:
        parent->update();
        break;
    default:
        break;
    }
}
