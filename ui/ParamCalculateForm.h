#ifndef PARAMCALCULATEFORM_H
#define PARAMCALCULATEFORM_H

#include <QWidget>
#include <QTextBrowser>
#include <QTreeWidgetItem>
#include <models/MivarModel.h>
#include <services/ParamCalculator.h>
#include "ResetableWidget.h"

namespace Ui {
class ParamCalculateForm;
}

class ParamCalculateForm : public QWidget, public ResetableWidget
{
    Q_OBJECT

    struct classMapData {
        QTreeWidgetItem* widget;
        std::shared_ptr<MivarClass> mivarClass;
        classMapData(QTreeWidgetItem* wgt, std::shared_ptr<MivarClass> cls) : widget(wgt), mivarClass(cls) {}
    };

    struct paramMapData {
        QTreeWidgetItem* widget;
        std::shared_ptr<MivarParam> param;
        paramMapData(QTreeWidgetItem* wgt, std::shared_ptr<MivarParam> prm) : widget(wgt), param(prm) {}
    };

    Ui::ParamCalculateForm *ui;
    std::shared_ptr<MivarModel> m_model;
    std::map<QString, classMapData> m_classes;
    std::map<QString, paramMapData> m_params;
    ParamCalculator m_calculator;
    QTextBrowser* m_browser;

    void clearView();

    void configureClass(QTreeWidgetItem* mivarClassItem, const std::shared_ptr<MivarClass>& mivarClass);
    void configureParam(QTreeWidgetItem* paramItem, const std::shared_ptr<MivarParam>& param);
    void addClass(QTreeWidgetItem* parent, const std::shared_ptr<MivarClass>& mivarClass);
    void addParam(QTreeWidgetItem* parent, const std::shared_ptr<MivarParam>& mivarParam);
    void deleteStoredClassData(const std::shared_ptr<MivarClass>& mivarClass);

    void bindWidgets(QTreeWidgetItem* classItem);
    std::shared_ptr<MivarClass> getClassByItem(QTreeWidgetItem* classItem);
    std::shared_ptr<MivarParam> getParamByItem(QTreeWidgetItem* paramItem);
    void swapChilds(QTreeWidgetItem* item, int firstIdx, int secondIdx);
    void sortTree(QTreeWidgetItem* item);

    void printCalcResult(const std::vector<ParamCalculator::ParamCalcResult>& results,
                         const std::vector<QString>& outputs,
                         const std::map<QString, QString>& inputs);
protected slots:
    void onSubclassAdd(std::shared_ptr<MivarClass> parent);
    void onSubclassDelete();
    void onParamAdd(std::shared_ptr<MivarClass> parent);
    void onParamDelete(std::shared_ptr<MivarClass> parent);
    void onItemExpanded(QTreeWidgetItem* item);

    void onCalculateClick();
public:
    explicit ParamCalculateForm(QWidget *parent = nullptr);
    void setParamOutputWidget(QTextBrowser* browser);
    void setModel(std::shared_ptr<MivarModel> model);
    void reset() override;
    ~ParamCalculateForm();
};

#endif
