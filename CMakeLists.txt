cmake_minimum_required(VERSION 3.5)

project(MapProject LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QGIS_PATH /apps)
# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

find_package(Qt5 COMPONENTS Core Xml Widgets Sql PrintSupport REQUIRED)
link_directories(${QGIS_PATH}/lib)
if(ANDROID)
  add_library(MapProject SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
  )
else()
  add_executable(MapProject
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
  )
endif()
target_include_directories(MapProject PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src" ${QGIS_PATH}/include/qgis /usr/include/gdal)
target_link_libraries(MapProject PRIVATE Qt5::Core Qt5::Xml Qt5::Widgets Qt5::Sql Qt5::PrintSupport qgis_native qgis_core qgis_app qgis_gui gdal)
