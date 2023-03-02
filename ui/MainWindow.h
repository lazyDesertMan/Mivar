#ifndef MAINWINDOW2_H
#define MAINWINDOW2_H

#include <QMainWindow>
#include "ClassOptions.h"
#include "ParameterOptions.h"
#include "RelationOptions.h"
#include "RuleOptions.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    ClassOptions *classOpt = new ClassOptions;
    ParameterOptions *paramOpt = new ParameterOptions;
    RuleOptions *ruleOpt = new RuleOptions;
    RelationOptions *relativeOpt = new RelationOptions;

private slots:
    void on_exit_triggered();
    void on_loadFile_triggered();
public slots:
    // Вывод формы для редактирования класса
    void ShowClassEdit();
    // Вывод формы для редактирования правил
    void ShowRuleEdit();
    // Вывод формы для редактирования параметров
    void ShowParameterEdit();
    // Вывод формы для редактирования отношений
    void ShowRelativeEdit();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW2_H
