#include <QUuid>
#include <iostream>
#include <QDebug>
#include "ClassOptions.h"
#include "ui_ClassOptions.h"

ClassOptions::ClassOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClassOptions)
{
    ui->setupUi(this);
    connect(ui->editClassBtn, SIGNAL(clicked()), this, SLOT(EditClass()));
    connect(ui->addClassBtn, SIGNAL(clicked()), this, SLOT(AddClass()));
}

ClassOptions::~ClassOptions()
{
    delete ui;
}

void ClassOptions::reset()
{
}

void ClassOptions::setEditableClass(std::shared_ptr<MivarClass> mc, const std::shared_ptr<MivarClass>& parent)
{
    m_mivarClass = mc;
    ui->nameClass->setText(mc->name());
    if (parent.get() != nullptr)
        ui->parentClass->setText(parent->name());
    else
        ui->parentClass->setText("");
    ui->descriptionClass->setText(mc->description());
    ui->editClassBtn->setVisible(true);
    ui->addClassBtn->setVisible(false);
}

void ClassOptions::prepareToAddClass(std::shared_ptr<MivarClass> mc)
{
    m_mivarClass = mc;
    ui->nameClass->setText("");
    ui->descriptionClass->setText("");
    ui->editClassBtn->setVisible(false);
    ui->addClassBtn->setVisible(true);
    ui->parentClass->setText(m_mivarClass->name());
}

// Редактирование классов
void ClassOptions::EditClass()
{
    if(ui->nameClass->text().size())
        m_mivarClass->setName(ui->nameClass->text());
    if(ui->descriptionClass->toPlainText().size())
        m_mivarClass->setDescription(ui->descriptionClass->toPlainText());
}

void ClassOptions::AddClass()
{
    if (ui->nameClass->text().size() > 0) {
        QString id = QUuid::createUuid().toString();
        QString name = ui->nameClass->text();
        QString description = ui->descriptionClass->toPlainText();
        std::shared_ptr<MivarClass> mc = std::make_shared<MivarClass>();
        mc->setId(id);
        mc->setName(name);
        mc->setDescription(description);
        m_mivarClass->addSubclass(mc);

    }
}

