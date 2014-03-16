cmake_minimum_required(VERSION 2.8.9)

project(Mapping)

##################
# qt5

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5Widgets)

#################
#
set(EXE_NAME Mapping)

set(SRC main.cpp
    Window.h Window.cpp
    )

add_executable(${EXE_NAME} ${SRC})

target_link_libraries(
        ${EXE_NAME}
)

qt5_use_modules(${EXE_NAME} Gui Widgets)
