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
    connect(ui->displayProject, &ClassTree::addParamEvent, this, &MainWindow::ShowAddParameterForm);
    connect(ui->saveFile, &QAction::triggered, this, &MainWindow::saveModel);

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
        m_fileSavePath = pathFile;
        m_model = ModelLoader::load(pathFile);
        ui->displayProject->DisplayMivar(m_model);
        ui->displayRelative->DisplayMivar(m_model);
    }
}

void MainWindow::saveModel() {
    if(m_fileSavePath.size())
        ModelLoader::save(m_model, m_fileSavePath);
}

void MainWindow::ShowClassEdit() {

}

// Вывод формы для редактирования класса
void MainWindow::ShowClassEditForm(const std::shared_ptr<MivarClass> &mivarClass, const std::shared_ptr<MivarClass>& parent) {
    ui->ClassOpt->setEditableClass(mivarClass, parent);
    ui->centralWidget->setCurrentWidget(ui->ClassOpt);
    activeWidget = ui->ClassOpt;
}

void MainWindow::ShowNewSubclassForm(const std::shared_ptr<MivarClass> &mivarClass) {
    activeWidget->reset();
    ui->ClassOpt->prepareToAddClass(mivarClass);
    ui->centralWidget->setCurrentWidget(ui->ClassOpt);
    activeWidget = ui->ClassOpt;
}

// Вывод формы для редактирования правила
void MainWindow::ShowRuleEdit() {
    activeWidget->reset();
    ui->centralWidget->setCurrentWidget(ui->RuleOpt);
    activeWidget = ui->RuleOpt;
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
    ui->ParamOpt->setEditableParam(m_model, param);
    ui->centralWidget->setCurrentWidget(ui->ParamOpt);
    activeWidget = ui->ParamOpt;
}

void MainWindow::ShowAddParameterForm(const std::shared_ptr<MivarClass>& mivarClass) {
    activeWidget->reset();
    ui->ParamOpt->prepareToAddParam(m_model, mivarClass);
    ui->centralWidget->setCurrentWidget(ui->ParamOpt);
    activeWidget = ui->ParamOpt;
}
