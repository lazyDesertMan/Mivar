#ifndef MAINWINDOW2_H
#define MAINWINDOW2_H

#include <QMainWindow>
#include <models/MivarClass.h>
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
    void ShowClassEdit();
    
    /**
     * @brief Отображение формы редактирования класса mivarClass
     * 
     * @param mivarClass 
     */
    void ShowClassEditForm(const std::shared_ptr<MivarClass>& mivarClass);

    /**
     * @brief Отображение формы добавления нового подкласса для mivarClass 
     * 
     * @param mivarClass Родительский класс
     */
    void ShowNewSubclassForm(const std::shared_ptr<MivarClass>& mivarClass);
    
    // Вывод формы для редактирования правил
    void ShowRuleEdit();
    
    // Вывод формы для редактирования параметров
    void ShowParameterEdit();
    
    /**
     * @brief Отображение формы добавления нового параметра в mivarClass
     * @param mivarClass Класс, в который добавляется параметр
     */
    void ShowAddParameterForm(const std::shared_ptr<MivarClass>& mivarClass);

    // Вывод формы для редактирования отношений
    void ShowRelativeEdit();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW2_H
