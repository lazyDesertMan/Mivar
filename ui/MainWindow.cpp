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

    connect(ui->editClass, &QAction::triggered, this, &MainWindow::ShowClassEdit);
    connect(ui->editRule, &QAction::triggered, this, &MainWindow::ShowRuleEdit);
    connect(ui->editRelative, &QAction::triggered, this, &MainWindow::ShowRelativeEdit);
    connect(ui->editParameter, &QAction::triggered, this, &MainWindow::ShowParameterEdit);
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
    std::shared_ptr<MivarModel> MivarM;
    QString pathFile = QFileDialog::getOpenFileName(this, "Выберите файл", QDir::currentPath(), "*.xml");
    if(pathFile.size() > 0){
        MivarM = ModelLoader::load(pathFile);
        ui->displayProject->DisplayMivar(MivarM);
    }
}
// Вывод формы для редактирования класса
void MainWindow::ShowClassEdit()
{
    //classOpt->show();
    ui->menuOptions->setCurrentIndex(0);
}
// Вывод формы для редактирования правила
void MainWindow::ShowRuleEdit()
{
    //ruleOpt->show();
    ui->menuOptions->setCurrentIndex(3);
}
// Вывод формы для редактирования отношения
void MainWindow::ShowRelativeEdit()
{
    //relativeOpt->show();
    ui->menuOptions->setCurrentIndex(2);
}
// Вывод формы для редактирования параметра
void MainWindow::ShowParameterEdit()
{
    //paramOpt->show();
    ui->menuOptions->setCurrentIndex(1);
}

