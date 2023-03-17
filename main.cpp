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
#include <ui/MainWindow.h>
#include <services/ModelLoader.h>
#include <services/ParamCalculator.h>
#include <services/GraphService.h>
#include <models/MivarRule.h>
#include <models/MivarRule.h>
#include <services/ParamCalculator.h>
#include <services/SingleJSEngine.h>

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
        std::shared_ptr<MivarModel> model = ModelLoader::load("./files/lab1_2.xml");
        ParamCalculator calculator;
        calculator.findWays(
            model, {
                {"e9368768-1124-4ee4-a5fb-920d2bbcd5a6", "0.4"},  // Вес Р1
                {"b7993d05-f7fd-482a-975d-94bdaa7128b8", "0.3"},  // Вес Р2
                {"4d4ac3dc-8e37-4387-98ec-1481f65cfba6", "0.2"},  // Вес Р3
                {"3f16b75f-fc43-4eb7-904a-569952a1e571", "0.1"},  // Вес Р4
                {"6a3ce3c4-93e5-45db-8a06-970d0f171a27", "0.4"},  // Вес Р5
                {"818a278d-09ed-42cf-8ae3-7cd1765eb361", "0.3"},  // Вес Р6
                {"ba210c9c-40ea-4d98-a1fe-9207daccb38a", "0.2"},  // Вес Р7
                {"e41290e1-dcc1-469a-b291-6ad2c0a757f7", "0.1"},  // Вес Р8
                {"4d2c2a53-2f18-4119-84fb-e3148d3cb157", "65000"},  // Доход с заявки, выполненной в срок
                {"729ed12b-29f9-430f-91f4-a826658ac5e8", "2500000"},  // Основные средства
                {"0895185f-c3a3-486e-b6c8-e56ed4c29001", "0.9"},  // Интенсивность нач. порт.
                {"b351c50c-ab96-459d-a6bc-94e0af50d237", "0.85"},  // Интенсивность вед. инж.
                {"ef803fa7-e0be-4a12-8828-dc797878d2e9", "0.79"},  // Интенсивность вед. тальм.
                {"b58c388d-5097-48f5-ba40-7c0fe601d744", "0.87"},  // Интенс. дир.
                {"ed5c6363-11ee-4a99-8de6-5f2145d2a1ee", "0.89"},  // Интенс. инж. отдел
                {"909aed7f-f702-437f-9fb2-3b2388f53134", "0.8"},  // Интенс. от. бух.
                {"87676dfc-f426-4cf5-a55d-e5f4a99e4b1a", "0.71"},  // Интенс. тальм. от.
                {"184527c9-52ac-4b51-a257-240dfaf06f25", "50"},  // Кол-во выполненных заявок
                {"f3485f11-056f-41c5-b5b4-8fa541571dd4", "4"},  // Кол-во заявок не в срок
                {"0d30fcd3-86b1-4594-9e83-c1ceef9486cd", "50"},  // Кол-во поступивших заявок

            }, {
                "32f0bde6-7ffc-4152-a9dc-6809f6f1c904"  // Интегральный показатель
            }
        );
    }
}
