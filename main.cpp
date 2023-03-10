#include <iostream>
#include <QApplication>
#include <QJSEngine>
#include "ui/MainWindow.h"
#include "services/ModelLoader.h"
#include "services/SingleJSEngine.h"
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
        std::shared_ptr<MivarModel> model = ModelLoader::load("./files/proj3.xml");
        ModelLoader::save(model, "./files/text.xml");
    }
}
