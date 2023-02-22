cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME Comp)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(SOURCES
    models/IObservable.h
    models/IObserver.h
    models/MivarModel.h
    models/MivarObject.h
    models/MivarRelation.h
    services/ModelLoader.h
    ui/ClassList.h
    ui/components/TreeView.h
    ui/MainContainer.h
    ui/MainWindow.h
    ui/Project.h
    models/IObservable.cpp
    models/MivarModel.cpp
    models/MivarObject.cpp
    models/MivarRelation.cpp
    services/ModelLoader.cpp
    ui/ClassList.cpp
    ui/components/TreeView.cpp
    ui/MainContainer.cpp
    ui/Project.cpp
    ui/MainWindow.cpp
    main.cpp
)
add_executable(${PROJECT_NAME} ${SOURCES})

find_package(QT REQUIRED NAMES Qt6 Qt5 COMPONENTS Widgets Xml Qml)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Xml Qml)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Xml Qt${QT_VERSION_MAJOR}::Qml)
