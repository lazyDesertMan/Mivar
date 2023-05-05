#ifndef CALCULATE_TREE_PARAM_H
#define CALCULATE_TREE_PARAM_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <models/MivarClass.h>

class CalculateTreeParamLabel : public QWidget {
    Q_OBJECT

    struct CalculateTreeParamObserver : public IObserver {
        CalculateTreeParamLabel* parent;
        void handle(int16_t code);
    };

    void update();

    std::shared_ptr<MivarParam>                 m_param;
    std::shared_ptr<CalculateTreeParamObserver> m_observer;
    
    QLabel*                                     m_label;
public:
    CalculateTreeParamLabel(std::shared_ptr<MivarParam> observedParam);
    ~CalculateTreeParamLabel();
signals:
    void onNameUpdate(std::shared_ptr<MivarParam> param);
};

#endif
