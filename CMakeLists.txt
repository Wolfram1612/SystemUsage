cmake_minimum_required(VERSION 3.5)

project(SystemUsage VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Charts)

add_executable(SystemUsage
    main
    watchers.h watchers.cpp
    views.h views.cpp
    charts.h charts.cpp
)

include(GNUInstallDirs)
install(TARGETS SystemUsage
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
target_link_libraries(SystemUsage PUBLIC
    Qt::Charts
    Qt::Widgets
)
