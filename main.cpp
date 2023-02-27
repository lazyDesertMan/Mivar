#include <iostream>
#include <QApplication>
#include <QJSEngine>
#include "ui/MainWindow.h"
#include "services/ModelLoader.h"
#include "services/SingleJSEngine.h"
#include "models/MivarRule.h"

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

void printRelation(const std::shared_ptr<MivarRelation>& rel) {
    std::cout << "| ID: " << rel->id().toStdString() << std::endl
        << "| Имя: " << rel->name().toStdString() << std::endl;
    if (rel->inputs().size()) {
        std::cout << "| Вход:" << std::endl;
        for(size_t i = 0; i < rel->inputs().size(); i++)
            std::cout << "| | " << rel->inputs()[i].name.toStdString() << " : "
                << rel->inputs()[i].type.toStdString() << std::endl;
    }
    if (rel->outputs().size()) {
        std::cout << "| Выход:" << std::endl;
        for(size_t i = 0; i < rel->outputs().size(); i++)
            std::cout << "| | " << rel->outputs()[i].name.toStdString() << " : "
                << rel->outputs()[i].type.toStdString() << std::endl;
    }
    std::cout  << "|" << std::endl;
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
        std::shared_ptr<MivarRelation> rel = model.relations()[0];
        MivarRule rule(rel);
        class tempObserver : public IObserver {
            void handle() { std::cout << "ok" << std::endl; }
        };
        std::shared_ptr<IObserver> observer = std::make_shared<tempObserver>();
        rule.addObserver(observer);
        rel->addInput({"test", "double"});


        std::cout << "Модель: " << model.name().toStdString() << std::endl << "Классы:" << std::endl;
        printClass(model.modelClass());

        std::cout << "Отношения:" << std::endl;
        for(size_t i = 0; i < model.relations().size(); i++) {
            printRelation(model.relations()[i]);
        }

    }
}
