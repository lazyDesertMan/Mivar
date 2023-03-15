#include <iostream>
#include <fstream>
#include <iterator>

// QT
#include <QApplication>
#include <QJSEngine>
#include <QDebug>

// Boost
#include <boost/graph/adjacency_list.hpp>

// Собственные
#include "ui/MainWindow.h"
#include "services/ModelLoader.h"
#include "services/ParamCalculator.h"
#include "services/GraphService.h"
#include "models/MivarRule.h"
#include "models/MivarRule.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(resources);
    bool isTesing = false;

    for (int i = 0; i < argc; i++) {
        if (QString::fromStdString(argv[i]) == "-D")
            isTesing = true;
    }
    
    if (!isTesing) {
        MainWindow container;
        container.show();
        app.exec();
    }
    else {
        //std::shared_ptr<MivarModel> model = ModelLoader::load("./files/lab1_2.xml");
        std::shared_ptr<MivarModel> model = ModelLoader::load("./files/proj3.xml");
        model->removeRelation("076991f6-9575-496f-b3ea-b558d6ae4262");
        qDebug() << "clear";
    }
}
