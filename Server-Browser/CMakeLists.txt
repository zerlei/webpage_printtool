cmake_minimum_required(VERSION 3.14)

project(PrintWebPage LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Drogon)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt6 REQUIRED COMPONENTS Core PrintSupport WebSockets WebEngineCore WebEngineWidgets Sql Gui)

aux_source_directory(server SERVER)


add_executable(PrintWebPage 

 WIN32
  main.cpp
  webprinter.h
  webprinter.cpp
  printdatabase.h
  printdatabase.cpp
  printmsgstation.h
  printmsgstation.cpp
  websoc.h
  websoc.cpp
  ${SERVER}
)
target_link_libraries(PrintWebPage
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::PrintSupport
    Qt${QT_VERSION_MAJOR}::WebEngineCore
    Qt${QT_VERSION_MAJOR}::WebEngineWidgets
    Qt${QT_VERSION_MAJOR}::Sql
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::WebSockets
    ${DROGON_LIBRARIES})
add_link_options(PrintWebPage PRIVATE -mwindows)
