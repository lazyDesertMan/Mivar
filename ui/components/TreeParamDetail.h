#ifndef TREE_PARAM_DETAIL_H
#define TREE_PARAM_DETAIL_H

#include <QLabel>
#include <models/MivarClass.h>

class TreeParamDetail : public QWidget {
    Q_OBJECT

    struct ParamObserver : IObserver {
        TreeParamDetail* parent;
        void handle(int16_t code);
    };

    std::shared_ptr<ParamObserver> m_observer;
    std::shared_ptr<MivarParam> m_param;

    QLabel*  m_icon;
    QLabel*  m_name;
public:
    const std::shared_ptr<MivarParam> getParam() const;
    void update();

    TreeParamDetail(std::shared_ptr<MivarParam> observedParam);
    ~TreeParamDetail();
};

#endif
