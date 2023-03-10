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
    services/ModelLoader.h
    services/SingleJSEngine.h
    ui/MainWindow.h
    ui/MainWindow.ui
    models/IObservable.cpp
    models/MivarClass.cpp
    models/MivarModel.cpp
    models/MivarObject.cpp
    models/MivarRelation.cpp
    models/MivarRule.cpp
    services/ModelLoader.cpp
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
    ui/components/TreeClassDetail.h
    ui/components/TreeClassDetail.cpp
    ui/components/TreeParamDetail.h
    ui/components/TreeParamDetail.cpp
    ui/ResetableWidget.h
    main.cpp
    resources.qrc
)
add_executable(${PROJECT_NAME} ${SOURCES})

find_package(QT REQUIRED NAMES Qt6 Qt5 COMPONENTS Widgets Xml Qml)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Xml Qml)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Xml Qt${QT_VERSION_MAJOR}::Qml)
