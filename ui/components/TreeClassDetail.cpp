#include <QDebug>
#include <QBoxLayout>
#include <QDebug>
#include <QMouseEvent>
#include "TreeClassDetail.h"

ClassActions::ClassActions(std::shared_ptr<MivarClass> mivarClass, QWidget* parent) : QWidget(parent), m_menu(this) {
    m_class = mivarClass;
    
    m_addParamAct = new QAction("Добавить параметр", this);
    connect(m_addParamAct, &QAction::triggered, this, &ClassActions::onAddParamClick);
    m_addSubclassAct = new QAction("Добавить подкласс", this);
    connect(m_addSubclassAct, &QAction::triggered, this, &ClassActions::onAddSubclassClick);
    m_editAct = new QAction("Изменить", this);
    connect(m_editAct, &QAction::triggered, this, &ClassActions::onEditClick);
    m_removeAct = new QAction("Удалить", this);
    connect(m_removeAct, &QAction::triggered, this, &ClassActions::onRemoveClick);
    m_menu.addActions({m_addParamAct, m_addSubclassAct, m_editAct, m_removeAct});
}

void ClassActions::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::MouseButton::RightButton)
        m_menu.exec(QCursor::pos());
}

void ClassActions::onAddParamClick() {
    emit addParamClick(m_class);
}

void ClassActions::onAddSubclassClick() {
    emit addSubclassClick(m_class);
}

void ClassActions::onEditClick() {
    emit editClick(m_class);
}

void ClassActions::onRemoveClick() {
    emit removeClick(m_class);
}

TreeClassDetailType::TreeClassDetailType(std::shared_ptr<MivarClass> mivarClass, QWidget* parent) : ClassActions(mivarClass, parent) {
    m_type = new QLabel(this);
    m_type->setText("Класс");
}

const std::shared_ptr<MivarClass> TreeClassDetail::getClass() const {
    return m_class;
}

void TreeClassDetail::update() {
    m_name->setText(m_class->name());
}

TreeClassDetail::TreeClassDetail(std::shared_ptr<MivarClass> observedClass, QWidget* parent) :
    ClassActions(observedClass, parent)
{
    m_observer = std::make_shared<ClassObserver>();
    m_observer->parent = this;
    m_class->addObserver(m_observer);
    
    QHBoxLayout* layout = new QHBoxLayout(this);
    setLayout(layout);
    m_name = new QLabel(this);
    m_name->setText(m_class->name());

    m_icon = new QLabel(this);
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
