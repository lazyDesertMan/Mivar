#include <iostream>
#include <QApplication>
#include <QJSEngine>
#include "ui/MainWindow.h"
#include "services/ModelLoader.h"
#include "services/SingleJSEngine.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
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
        QJSEngine& engine = SingleJSEngine::engine();

        MivarModel model = ModelLoader::load("./files/proj3.xml");
        std::shared_ptr<MivarRelation> rel = model.relations()[1];
        std::cout << rel->name().toStdString() << std::endl;
        QJSValue func = engine.evaluate(rel->toJSFunction());
        QJSValueList params;
        params << 1 << 2 << 5;
        QJSValue res = func.call(params);
        std::cout << res.toString().toStdString() << std::endl;
    }
}
