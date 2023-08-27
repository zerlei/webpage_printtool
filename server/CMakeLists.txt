cmake_minimum_required(VERSION 3.20)

project(PrintWebPage LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Drogon)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt6 REQUIRED COMPONENTS Core PrintSupport WebSockets WebEngineCore WebEngineWidgets Sql Gui Widgets Pdf)

# aux_source_directory(server SERVER)


add_executable(PrintWebPage 

  WIN32
  main.cpp

  printer/webprinter.h
  printer/webprinter.cpp
  printer/printmsgstation.h
  printer/printmsgstation.cpp

  data_sqlite/printdatabase.h
  data_sqlite/printdatabase.cpp

  net_interface/clientwebsocket.cpp
  net_interface/clientwebsocket.h
  net_interface/printcontroller.cpp
  net_interface/printcontroller.h
  net_interface/printwebsocket.cpp
  net_interface/printwebsocket.h

  gui/bg.qrc
  # gui/movewidget.cpp
  # gui/movewidget.h
  # gui/printwidget.cpp
  # gui/printwidget.h
  # gui/printwidget.ui
  gui/app_win32.rc
)
target_link_libraries(PrintWebPage
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::PrintSupport
    Qt${QT_VERSION_MAJOR}::WebEngineCore
    Qt${QT_VERSION_MAJOR}::WebEngineWidgets
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::WebSockets
    Qt${QT_VERSION_MAJOR}::Pdf
    ${DROGON_LIBRARIES})
add_link_options(PrintWebPage PRIVATE -mwindows)
