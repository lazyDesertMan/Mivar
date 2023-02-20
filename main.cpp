#include <iostream>
#include <QApplication>
#include <QJSEngine>
#include "ui/MainContainer.h"
#include "services/ModelLoader.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QJSEngine engine;

    MivarModel model = ModelLoader::load("./files/proj3.xml");
    /*std::cout << "ID:        " << model.id().toStdString() << std::endl
              << "Имя:       " << model.name().toStdString() << std::endl
              << "Описание:  " << model.description().toStdString() << std::endl
              << "Отношения: " << std::endl;
    for (auto r : model.relations()) {
        std::cout << "\tID:       " << r.id().toStdString() << std::endl
                  << "\tИмя:      " << r.name().toStdString() << std::endl
                  << "\tТип:      " << r.type().toStdString() << std::endl
                  << "\tОписание: " << r.description().toStdString() << std::endl
                  << "\tВход:     " << std::endl;
        for(auto param : r.inputs())
            std::cout << "\t\t" << param.name().toStdString() << " : " << param.type().toStdString() << std::endl;
        std::cout << "\tВыход:    " << std::endl;
        for(auto param : r.outputs())
            std::cout << "\t\t" << param.name().toStdString() << " : " << param.type().toStdString() << std::endl;
        std::cout << "\tКод:      " << r.code().toStdString() << std::endl << std::endl;
    }*/
    std::shared_ptr<MivarRelation> rel = model.relations()[1];
    std::cout << rel->name().toStdString() << std::endl;
    QJSValue func = engine.evaluate(rel->toJSFunction());
    QJSValueList params;
    params << 1 << 2 << 5;
    QJSValue res = func.call(params);
    std::cout << res.toString().toStdString() << std::endl;
    /*MainContainer container;
    container.show();
    app.exec();*/
}
