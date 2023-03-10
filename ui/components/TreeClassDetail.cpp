#include <QDebug>
#include <QBoxLayout>
#include <QDebug>
#include <QMouseEvent>
#include "TreeClassDetail.h"

/*---ClassActions---*/

ClassActions::ClassActions(std::shared_ptr<MivarClass> mivarClass, bool isRootClass) : m_menu(this) {
    m_class = mivarClass;
    
    m_addParamAct = new QAction("Добавить параметр", this);
    connect(m_addParamAct, &QAction::triggered, this, &ClassActions::onAddParamClick);
    m_addSubclassAct = new QAction("Добавить подкласс", this);
    connect(m_addSubclassAct, &QAction::triggered, this, &ClassActions::onAddSubclassClick);
    m_editAct = new QAction("Изменить", this);
    connect(m_editAct, &QAction::triggered, this, &ClassActions::onEditClick);
    if (!isRootClass) {
        m_removeAct = new QAction("Удалить", this);
        connect(m_removeAct, &QAction::triggered, this, &ClassActions::onRemoveClick);
        m_menu.addActions({m_addParamAct, m_addSubclassAct, m_editAct, m_removeAct});
    }
    else
        m_menu.addActions({m_addParamAct, m_addSubclassAct, m_editAct});
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

const std::shared_ptr<MivarClass> ClassActions::getClass() const {
    return m_class;
}

/*---TreeClassDetailType---*/

TreeClassDetailType::TreeClassDetailType(std::shared_ptr<MivarClass> mivarClass, bool isRootClass) : ClassActions(mivarClass, isRootClass) {
    m_type = new QLabel(this);
    m_type->setText("Класс");

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setAlignment(Qt::AlignVCenter);
    layout->addWidget(m_type);
    setLayout(layout);
}

/*---TreeClassDetail---*/

void TreeClassDetail::paramAdded()
{
    emit onParamAdd(m_class);
}

void TreeClassDetail::update() {
    m_name->setText(m_class->name());
}

TreeClassDetail::TreeClassDetail(std::shared_ptr<MivarClass> observedClass, bool isRootClass) :
    ClassActions(observedClass, isRootClass)
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
    m_icon->setAlignment(Qt::AlignVCenter);

    layout->addWidget(m_icon);
    layout->addWidget(m_name);
    layout->setStretch(0, 0);
    layout->setStretch(1, 1);
    layout->setAlignment(Qt::AlignVCenter);

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
    case MivarClass::EventCode::EC_PARAM_ADD:
        parent->paramAdded();
        break;
    default:
        break;
    }
}
