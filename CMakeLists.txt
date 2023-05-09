cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME Comp)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include_directories("./")

set(SOURCES
    models/IObservable.h
    models/IObserver.h
    models/MivarClass.h
    models/MivarModel.h
    models/MivarObject.h
    models/MivarRelation.h
    models/MivarRule.h
    services/GraphService.h
    services/ModelLoader.h
    services/ParamCalculator.h
    services/SingleJSEngine.h
    ui/MainWindow.h
    ui/MainWindow.ui
    models/IObservable.cpp
    models/MivarClass.cpp
    models/MivarModel.cpp
    models/MivarObject.cpp
    models/MivarRelation.cpp
    models/MivarRule.cpp
    services/GraphService.cpp
    services/ModelLoader.cpp
    services/ParamCalculator.cpp
    ui/MainWindow.cpp
    ui/ClassTree.cpp
    ui/ClassTree.h
    ui/ClassTree.ui
    ui/RelationOptions.cpp
    ui/RelationOptions.h
    ui/RelationOptions.ui
    ui/ParameterOptions.cpp
    ui/ParameterOptions.h
    ui/ParameterOptions.ui
    ui/RuleOptions.cpp
    ui/RuleOptions.h
    ui/RuleOptions.ui
    ui/ClassOptions.cpp
    ui/ClassOptions.h
    ui/ClassOptions.ui
    ui/HomeWidget.cpp
    ui/HomeWidget.h
    ui/HomeWidget.ui
    ui/components/modelTree/TreeClassDetail.h
    ui/components/modelTree/TreeClassDetail.cpp
    ui/components/modelTree/TreeParamDetail.h
    ui/components/modelTree/TreeParamDetail.cpp
    ui/components/relationsTree/TreeRelationDetail.h
    ui/components/relationsTree/TreeRelationDetail.cpp
    ui/components/relationsTree/TreeRuleDetail.h
    ui/components/relationsTree/TreeRuleDetail.cpp
    ui/components/calculateTree/CalculateTreeClass.h
    ui/components/calculateTree/CalculateTreeClass.cpp
    ui/components/calculateTree/CalculateTreeParam.h
    ui/components/calculateTree/CalculateTreeParam.cpp
    ui/components/NameIdListModel.h
    ui/components/NameIdListModel.cpp
    ui/ResetableWidget.h
    ui/RelationTree.h
    ui/RelationTree.cpp
    ui/RelationTree.ui
    ui/ParamCalculateForm.h
    ui/ParamCalculateForm.cpp
    ui/ParamCalculateForm.ui
    main.cpp
    resources.qrc
)

find_package(Boost REQUIRED COMPONENTS graph)
find_package(Qt5 REQUIRED COMPONENTS Widgets Xml Qml WebEngineWidgets)

add_executable(${PROJECT_NAME} ${SOURCES})
if(Boost_FOUND AND Qt5_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE Boost::graph)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets Qt5::Xml Qt5::Qml Qt5::WebEngineWidgets)
endif()
