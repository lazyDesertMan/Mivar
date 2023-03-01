#include <QDebug>
#include <QBoxLayout>
#include "TreeClassDetail.h"

const std::shared_ptr<MivarClass> TreeClassDetail::getClass() const {
    return m_class;
}

QWidget* TreeClassDetail::name() {
    return m_container;
}

void TreeClassDetail::update() {
    m_name->setText(m_class->name());
}

TreeClassDetail::TreeClassDetail(std::shared_ptr<MivarClass> observedClass) {
    m_observer = std::make_shared<ClassObserver>();
    m_observer->parent = this;
    m_class = observedClass;
    m_class->addObserver(m_observer);
    
    m_container = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(m_container);
    m_container->setLayout(layout);
    m_name = new QLabel(m_container);
    m_name->setText(m_class->name());

    m_icon = new QLabel(m_container);
    QPixmap classIcon("://image/res/icons/class.svg");
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

TreeClassDetail::~TreeClassDetail() {
    m_class->removeObserver(m_observer);
    delete m_container;
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
