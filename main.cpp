#include <iostream>
#include <QApplication>
#include <QJSEngine>
#include "ui/MainWindow.h"
#include "services/ModelLoader.h"
#include "services/SingleJSEngine.h"

void printClass(const MivarClass& mClass, int deep = 1) {
    for(int i = 0; i < deep; i++)
        std::cout << "| ";
    std::cout << "ID: " << mClass.id().toStdString() << std::endl;
    for(int i = 0; i < deep; i++)
        std::cout << "| ";
    std::cout << "Имя: " << mClass.name().toStdString() << std::endl;
    if (mClass.subclasses().size()) {
        for(int i = 0; i < deep; i++)
            std::cout << "| ";
        std::cout << "Подклассы:" << std::endl;
        for(size_t i = 0; i < mClass.subclasses().size(); i++)
            printClass(mClass.subclasses()[i], deep + 1);
    }
    for(int i = 0; i < deep; i++)
        std::cout << "| ";
    std::cout << std::endl;
}

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
        std::cout << "Модель: " << model.name().toStdString() << std::endl << "Классы:" << std::endl;
        printClass(model.modelClass());

        std::cout << "Отношения:" << std::endl;
        for(size_t i = 0; i < model.relations().size(); i++) {
            std::cout << "| " << model.relations()[i]->name().toStdString() << std::endl;
        }
    }
}
