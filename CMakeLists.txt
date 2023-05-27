cmake_minimum_required(VERSION 3.25)
project(fpToVTF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)
set(CMAKE_INSTALL_RPATH $ORIGIN)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (UNIX)
    set(QT_INCLUDE "/usr/include/x86_64-linux-gnu/qt5")
    if (NOT DEFINED QT_BASEDIR)
        set(QT_BASEDIR "$ENV{HOME}/Qt/5.15.2/gcc_64/")
    endif ()
    if (DEFINED QT_BASEDIR)
        set(CMAKE_PREFIX_PATH "${QT_BASEDIR}/lib/cmake")
    else ()
        message(FATAL_ERROR "--!@ Please define your QT install dir with -DQT_BASEDIR=C:/your/qt5/here")
    endif ()
elseif (WIN32)
    # On Windows, the install dir must be specified, as it's not standardized
    if (DEFINED QT_BASEDIR)
        # Add it to the prefix path so find_package can find it
        list(APPEND CMAKE_PREFIX_PATH ${QT_BASEDIR})
        set(QT_INCLUDE ${QT_BASEDIR}/include)
        # CMake has an odd policy that links a special link lib for Qt on newer versions of CMake. Enable it so we don't get spammed, and I get to write less
        cmake_policy(SET CMP0020 NEW)
    else ()
        message(FATAL_ERROR "--!@ Please define your QT install dir with -DQT_BASEDIR=C:/your/qt5/here")
    endif ()
endif ()

message("Using ${QT_INCLUDE} as our Qt include dir")


find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_subdirectory(libs/VTFLib)

add_executable(${PROJECT_NAME} src/main.cpp
        src/MainWindow.cpp
        src/MainWindow.h)

target_link_libraries(${PROJECT_NAME}
        Qt::Core
        Qt::Gui
        Qt::Widgets
        vtflib
)

