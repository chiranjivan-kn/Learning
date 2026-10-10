cmake_minimum_required(VERSION 3.16)

project(QtCANPOC LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets SerialBus)

qt_standard_project_setup()

qt_add_executable(QtCANPOC
    main.cpp
)

target_link_libraries(QtCANPOC PRIVATE
    Qt6::Widgets
    Qt6::SerialBus
)
