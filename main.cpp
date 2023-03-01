#include <iostream>
#include <QApplication>
#include <QJSEngine>
#include "ui/MainWindow.h"
#include "services/ModelLoader.h"
#include "services/SingleJSEngine.h"
#include "models/MivarRule.h"

void printRules(const std::shared_ptr<MivarClass>& mClass, int deep = 1) {
    for(int i = 0; i < deep; i++)
        std::cout << "| ";
    std::cout << "Правила:" << std::endl;
    for (size_t i = 0; i < mClass->rules().size(); i++) {
        for(int i = 0; i < deep + 1; i++)
                std::cout << "| ";
        std::cout << "ID: " << mClass->rules()[i]->id().toStdString() << std::endl;
        for(int i = 0; i < deep + 1; i++)
                std::cout << "| ";
        std::cout << "Имя: " << mClass->rules()[i]->name().toStdString() << std::endl;
        if (mClass->rules()[i]->params().size()) {
            for(int i = 0; i < deep + 1; i++)
                std::cout << "| ";
            std::cout << "Параметры:" << std::endl;
            for(auto param : mClass->rules()[i]->params()) {
                for(int i = 0; i < deep + 2; i++)
                    std::cout << "| ";
                std::cout << param.first.toStdString() << " : " << param.second.toStdString() << std::endl;
            }
        }
        for(int i = 0; i < deep + 1; i++)
            std::cout << "| ";
        std::cout << std::endl;
    }
}

void printParams(const std::shared_ptr<MivarClass>& mClass, int deep = 1) {
    for(int i = 0; i < deep; i++)
        std::cout << "| ";
    std::cout << "Параметры:" << std::endl;
    for (size_t i = 0; i < mClass->params().size(); i++) {
        for(int i = 0; i < deep + 1; i++)
                std::cout << "| ";
        std::cout << "ID: " << mClass->params()[i]->id().toStdString() << std::endl;
        for(int i = 0; i < deep + 1; i++)
                std::cout << "| ";
        std::cout << "Имя: " << mClass->params()[i]->name().toStdString() << std::endl;
        for(int i = 0; i < deep + 1; i++)
                std::cout << "| ";
        std::cout << "Тип: " << mClass->params()[i]->type() << std::endl;
        for(int i = 0; i < deep + 1; i++)
            std::cout << "| ";
        std::cout << std::endl;
    }
}

void printClass(const std::shared_ptr<MivarClass>& mClass, int deep = 1) {
    for(int i = 0; i < deep; i++)
        std::cout << "| ";
    std::cout << "ID: " << mClass->id().toStdString() << std::endl;
    for(int i = 0; i < deep; i++)
        std::cout << "| ";
    std::cout << "Имя: " << mClass->name().toStdString() << std::endl;
    if (mClass->subclasses().size()) {
        for(int i = 0; i < deep; i++)
            std::cout << "| ";
        std::cout << "Подклассы:" << std::endl;
        for(size_t i = 0; i < mClass->subclasses().size(); i++)
            printClass(mClass->subclasses()[i], deep + 1);
    }
    if (mClass->rules().size())
        printRules(mClass, deep);
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
        QJSEngine& engine = SingleJSEngine::engine();

        std::shared_ptr<MivarModel> model = ModelLoader::load("./files/proj3.xml");

        std::cout << "Модель: " << model->name().toStdString() << std::endl << "Классы:" << std::endl;
        printClass(model->modelClass());

        std::cout << "Отношения:" << std::endl;
        for(size_t i = 0; i < model->relations().size(); i++) {
            printRelation(model->relations()[i]);
        }
    }
}
