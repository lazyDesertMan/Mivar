#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <models/MivarModel.h>
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

    ResetableWidget* activeWidget;

private slots:
    void on_exit_triggered();
    void on_loadFile_triggered();
    void saveModel();
    void testingModel();
    void on_showGraph_triggered();
    void newModel();

public slots:
    void afterLoad(bool);

    void ShowClassEdit();
    
    /**
     * @brief Отображение формы редактирования класса mivarClass
     * 
     * @param mivarClass 
     */
    void ShowClassEditForm(const std::shared_ptr<MivarClass>& mivarClass, const std::shared_ptr<MivarClass>& parent);

    /**
     * @brief Отображение формы добавления нового подкласса для mivarClass 
     * 
     * @param mivarClass Родительский класс
     */
    void ShowNewSubclassForm(const std::shared_ptr<MivarClass>& mivarClass);
    
    // Вывод формы для редактирования правил
    void ShowRuleEdit();
    
    // Вывод формы для редактирования параметров
    void ShowParameterEdit(std::shared_ptr<MivarParam> param);
    
    /**
     * @brief Отображение формы добавления нового параметра в mivarClass
     * @param mivarClass Класс, в который добавляется параметр
     */
    void ShowAddParameterForm(const std::shared_ptr<MivarClass>& mivarClass);
    /**
     * @brief Отображение формы добавления нового отношения в MivarModel
     * @param MivarModel Модель, в которую добавляется отнеошение
     */
    void ShowAddRelativeForm();
    /**
     * @brief Отображение формы добавления нового правила в MivarModel
     * @param MivarModel Модель, в которую добавляется правила
     */
    void ShowAddRuleForm(std::shared_ptr<MivarClass> parentClass);

    // Вывод формы для редактирования отношений
    void ShowRelativeEdit();

private:
    Ui::MainWindow *ui;
    std::shared_ptr<MivarModel> m_model;
    QString m_fileSavePath;
};

#endif // MAINWINDOW_H
