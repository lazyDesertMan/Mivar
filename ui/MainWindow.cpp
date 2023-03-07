#include "MainWindow.h"
#include "RelationOptions.h"
#include "ui_MainWindow.h"
#include "services/ModelLoader.h"
#include "models/MivarModel.h"
#include <QFileDialog>
#include <iostream>
#include <QDebug>
#include <QWidget>
#include <QAction>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->splitter_1->setStretchFactor(0, 1);
    ui->splitter_1->setStretchFactor(1, 2);
    ui->splitter_2->setStretchFactor(1, 1);
    ui->splitter_2->setStretchFactor(0, 10);
    ui->splitter_3->setStretchFactor(1, 1);
    ui->splitter_3->setStretchFactor(0, 10);

    connect(ui->displayProject, &ClassTree::editClassEvent, this, &MainWindow::ShowClassEditForm);
    connect(ui->displayProject, &ClassTree::addClassEvent, this, &MainWindow::ShowNewSubclassForm);
    connect(ui->displayProject, &ClassTree::editParamEvent, this, &MainWindow::ShowParameterEdit);

    activeWidget = ui->HomeOpt;
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Выход из приложения
void MainWindow::on_exit_triggered()
{
    QApplication::quit();
}

// Загрузка *.xml файла
void MainWindow::on_loadFile_triggered()
{
    m_model.reset();
    QString pathFile = QFileDialog::getOpenFileName(this, "Выберите файл", QDir::currentPath(), "*.xml");
    if(pathFile.size() > 0) {
        m_model = ModelLoader::load(pathFile);
        ui->displayProject->DisplayMivar(m_model);
    }
}
// Вывод формы для редактирования класса
void MainWindow::ShowClassEdit() {
    activeWidget->reset();
    ui->centralWidget->setCurrentWidget(ui->ClassOpt);
    activeWidget = ui->ClassOpt;
}

void MainWindow::ShowClassEditForm(const std::shared_ptr<MivarClass> &mivarClass) {
    qDebug() << mivarClass->name();
}

void MainWindow::ShowNewSubclassForm(const std::shared_ptr<MivarClass> &mivarClass) {
    qDebug() << mivarClass->name();
}

// Вывод формы для редактирования правила
void MainWindow::ShowRuleEdit() {
    activeWidget->reset();
    ui->centralWidget->setCurrentWidget(ui->RuleOpt);
    activeWidget = ui->RuleOpt;
}

void MainWindow::ShowAddParameterForm(const std::shared_ptr<MivarClass>& mivarClass) {
    qDebug() << mivarClass->name();
}

// Вывод формы для редактирования отношения
void MainWindow::ShowRelativeEdit() {
    activeWidget->reset();
    ui->centralWidget->setCurrentWidget(ui->RelativeOpt);
    activeWidget = ui->RelativeOpt;
}

// Вывод формы для редактирования параметра
void MainWindow::ShowParameterEdit(std::shared_ptr<MivarParam> param) {
    activeWidget->reset();
    ui->ParamOpt->setEditableParam(param);
    ui->centralWidget->setCurrentWidget(ui->ParamOpt);
    activeWidget = ui->ParamOpt;
}

