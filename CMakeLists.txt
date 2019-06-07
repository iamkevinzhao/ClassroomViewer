CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(ClassroomViewer C CXX)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Widgets)
find_package(Qt5Xml)
find_package(Qt5Network)

add_executable(
  ${PROJECT_NAME}
    src/main.cpp
    src/main_window.cpp
    src/main_window.h
    src/main_window.ui
    src/map_widget.cpp
    src/map_widget.h
    src/map_widget.ui
    src/models.cpp
    src/models.h
    src/profile.cpp
    src/profile.h
    src/server.cpp
    src/server.h
#    src/policy.cpp
#    src/policy.h
)

target_link_libraries(
  ${PROJECT_NAME}
    Qt5::Widgets
    Qt5::Xml
    Qt5::Network
)
